"""
Reference with modifications from https://github.com/s7manth/cs3203-spa/blob/master/Team02/Tests02/RunTests.py
"""

import os
import glob
import time
import shutil
from collections import Counter
import subprocess


class Runner:
    WINDOWS_OS_NAME = 'nt'
    MAC_OS_NAME = 'posix'
    SUCCESS_EXIT_CODE = 0
    FAILURE_CLOSING_TAG = '</failed>'
    SUCCESS_TAG = '<passed/>'
    TIMEOUT_TAG = '<timeout/>'
    PASS_DIRECTORY = os.path.join(os.getcwd(), "Pass")
    FAIL_DIRECTORY = os.path.join(os.getcwd(), "Fail")
    TEMP_XML_FILENAME = "temp.xml"
    LOG_FILE = "log.txt"

    def __init__(self):
        self.TOTAL_TESTS = 0
        self.TOTAL_PASSED_TESTS = 0
        self.TOTAL_FAILED_TESTS = 0
        self.TOTAL_TIMEOUT_TESTS = 0
        self.AUTOTESTER_FAIL = 0
        self.make_result_directory()

    @staticmethod
    def make_path_suitable(command):
        return os.path.normpath(command)

    def find_autotester_executable(self):
        file_name = 'autotester.exe' if os.name == self.WINDOWS_OS_NAME else 'autotester'

        # Our Team16 directory is the parent directory of the current working directory
        team_code_dir = os.path.dirname(os.getcwd())

        paths = [os.path.join(root, file) for root, _, files in os.walk(team_code_dir) for file in files if
                 file == file_name]

        if len(paths) > 1:
            raise ValueError(f"More than 1 of {file_name} found in {team_code_dir} folder")

        if len(paths) == 0:
            raise Exception(f"Unable to find {file_name} in {team_code_dir} folder")

        return paths[0]

    def make_result_directory(self):
        for path in [self.PASS_DIRECTORY, self.FAIL_DIRECTORY]:
            shutil.rmtree(path, ignore_errors=True)
            os.makedirs(path)

    def get_autotester_parameters(self, folder_to_test_in):
        test_name_list = glob.glob(os.path.join(os.getcwd(), folder_to_test_in, "**", "*.txt"), recursive=True)

        source_tests = [name for name in test_name_list if name.endswith("source.txt")]
        query_tests = [name for name in test_name_list if name.endswith("queries.txt")]

        assert len(source_tests) == len(query_tests), "Mismatch between source and query files"

        parameters = []
        for source, query in zip(sorted(source_tests), sorted(query_tests)):
            base_source = os.path.basename(source)[:-10]
            base_query = os.path.basename(query)[:-11]

            assert base_source == base_query, f"Source: {base_source}, Query: {base_query}"

            test_name = os.path.basename(source)[:-10]

            test_dir = os.path.dirname(source)
            relative_path = os.path.join(folder_to_test_in,
                                         os.path.relpath(test_dir, os.path.join(os.getcwd(), folder_to_test_in)))

            parameters.append([source, query, test_name, relative_path])

        return parameters

    def check_output_xml(self, output_xml, test_name, relative_path):
        if not os.path.exists(output_xml):
            return "\n"

        with open(output_xml) as f:
            output = f.read()
            counts = Counter(output.split())
            passed_tests = counts[self.SUCCESS_TAG]
            failed_tests = counts[self.FAILURE_CLOSING_TAG]
            timeout_tests = counts[self.TIMEOUT_TAG]

        self.TOTAL_TESTS += (passed_tests + failed_tests + timeout_tests)
        self.TOTAL_PASSED_TESTS += passed_tests
        self.TOTAL_FAILED_TESTS += failed_tests
        self.TOTAL_TIMEOUT_TESTS += timeout_tests

        if failed_tests > 0 or timeout_tests > 0:
            destination = os.path.join(self.FAIL_DIRECTORY, relative_path)
        else:
            destination = os.path.join(self.PASS_DIRECTORY, relative_path)

        if not os.path.exists(destination):
            os.makedirs(destination)

        shutil.move(output_xml, os.path.join(destination, f"{test_name}out.xml"))
        shutil.copy("analysis.xsl", destination)

        return f"\nTest passed: {passed_tests}\nTest failed: {failed_tests}\nTest timeout: {timeout_tests}\n"

    import subprocess

    # ...

    def execute_autotester(self, autotester_filepath, parameters, redirect_output):
        source, query, test_name, relative_path = parameters

        # Wrap file paths with spaces in double quotes
        autotester_filepath = f'"{autotester_filepath}"'
        source = f'"{source}"'
        query = f'"{query}"'

        command = [autotester_filepath, source, query, self.TEMP_XML_FILENAME]
        command = self.make_path_suitable(" ".join(command))

        if redirect_output:
            with open(self.LOG_FILE, 'w') as log_file:
                result = subprocess.run(command, shell=True, stdout=log_file, stderr=subprocess.PIPE, text=True)
                exit_code = result.returncode
                error_message = result.stderr

                if exit_code != self.SUCCESS_EXIT_CODE:
                    self.AUTOTESTER_FAIL += 1
                    return f"Execution failed for {test_name[:-1]} with exit code: {exit_code}\nError Message: {error_message}"
        else:
            result = subprocess.run(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
            exit_code = result.returncode
            error_message = result.stderr

            if exit_code != self.SUCCESS_EXIT_CODE:
                return f"Execution failed for {test_name[:-1]} with exit code: {exit_code}\nError Message: {error_message}"

        return f"Execution completed for {test_name[:-1]} {self.check_output_xml(self.TEMP_XML_FILENAME, test_name, relative_path)}"

    def execute(self, folder_to_test_in, redirect_output=True):
        start_num_tests = self.TOTAL_TESTS
        autotester_filepath = self.find_autotester_executable()
        parameters = self.get_autotester_parameters(folder_to_test_in)

        test_report = ""
        for param in parameters:
            test_report += "\n" + self.execute_autotester(autotester_filepath, param, redirect_output)

        print(test_report)

        print(f"Total number of tests for {folder_to_test_in}: {self.TOTAL_TESTS - start_num_tests}")


if __name__ == "__main__":
    FOLDERS = ["Milestone1", "Milestone2", "Milestone3"]

    start_time = time.time()
    runner = Runner()
    for folder in FOLDERS:
        runner.execute(folder)

    print(f"Test statistics for {FOLDERS}:")
    print(f"Total passed tests: {runner.TOTAL_PASSED_TESTS}")
    print(f"Total failed tests: {runner.TOTAL_FAILED_TESTS}")
    print(f"Total timeout tests: {runner.TOTAL_TIMEOUT_TESTS}")
    print(f"Total autotester fail: {runner.AUTOTESTER_FAIL}")
    print(f"Total tests: {runner.TOTAL_TESTS}")

    print(f"Total time taken: {time.time() - start_time:.4f} seconds")

    if runner.TOTAL_FAILED_TESTS > 0 or runner.AUTOTESTER_FAIL > 0 or runner.TOTAL_TIMEOUT_TESTS > 0:
        raise Exception("Some tests failed, timed out or source couldn't be parsed!")
