"""
Reference with modifications from https://github.com/s7manth/cs3203-spa/blob/master/Team02/Tests02/RunTests.py
"""

import os
import glob
import time
import shutil
from collections import Counter


class Runner:
    WINDOWS_OS_NAME = 'nt'
    MAC_OS_NAME = 'posix'
    SUCCESS_EXIT_CODE = 0
    FAILURE_CLOSING_TAG = '</failed>'
    SUCCESS_TAG = '<passed/>'
    PASS_DIRECTORY = os.path.join(os.getcwd(), "Pass")
    FAIL_DIRECTORY = os.path.join(os.getcwd(), "Fail")
    TEMP_XML_FILENAME = "temp.xml"
    LOG_FILE = "log.txt"

    def __init__(self):
        self.TOTAL_TESTS = 0
        self.TOTAL_PASSED_TESTS = 0
        self.TOTAL_FAILED_TESTS = 0

    @staticmethod
    def make_path_suitable(command):
        return os.path.normpath(command)

    def find_autotester_executable(self):
        file_name = 'autotester.exe' if os.name == self.WINDOWS_OS_NAME else 'autotester'

        # Our Team16 directory is the parent directory of the current working directory
        team_code_dir = os.path.dirname(os.getcwd())

        executable_path = None

        for root, dirs, files in os.walk(team_code_dir):
            for file in files:
                if file == file_name:
                    executable_path = os.path.join(root, file)

        if executable_path is None:
            raise Exception(f"Unable to find {file_name} in {team_code_dir} folder")

        return executable_path

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

        self.TOTAL_TESTS += (passed_tests + failed_tests)
        self.TOTAL_PASSED_TESTS += passed_tests
        self.TOTAL_FAILED_TESTS += failed_tests

        if failed_tests > 0:
            destination = os.path.join(self.FAIL_DIRECTORY, relative_path)
        else:
            destination = os.path.join(self.PASS_DIRECTORY, relative_path)

        if not os.path.exists(destination):
            os.makedirs(destination)

        shutil.move(output_xml, os.path.join(destination, f"{test_name}out.xml"))
        shutil.copy("analysis.xsl", destination)

        return f"\nTest passed: {passed_tests}\nTest failed: {failed_tests}\n"

    def execute_autotester(self, autotester_filepath, parameters, redirect_output):
        source, query, test_name, relative_path = parameters

        command = [autotester_filepath, source, query, self.TEMP_XML_FILENAME]
        command = self.make_path_suitable(" ".join(command))

        if redirect_output:
            exit_code = os.system(command + f" > {self.LOG_FILE}")
        else:
            exit_code = os.system(command)

        if exit_code != self.SUCCESS_EXIT_CODE:
            return f"Execution failed for {test_name} with exit code: {exit_code}"

        return f"Execution completed for {test_name} {self.check_output_xml(self.TEMP_XML_FILENAME, test_name, relative_path)}"

    def execute(self, folder_to_test_in, redirect_output=True):
        autotester_filepath = self.find_autotester_executable()
        parameters = self.get_autotester_parameters(folder_to_test_in)

        self.make_result_directory()

        test_report = ""
        for param in parameters:
            test_report += "\n" + self.execute_autotester(autotester_filepath, param, redirect_output)

        print(test_report)


if __name__ == "__main__":
    start_time = time.time()
    runner = Runner()
    runner.execute("Milestone1")
    runner.execute("Milestone2")

    print(f"Test statistics:")
    print(f"Total passed tests: {runner.TOTAL_PASSED_TESTS}")
    print(f"Total failed tests: {runner.TOTAL_FAILED_TESTS}")
    print(f"Total tests: {runner.TOTAL_TESTS}")

    print(f"Total time taken: {time.time() - start_time:.4f} seconds")

    if runner.TOTAL_FAILED_TESTS > 0:
        raise Exception("Some tests failed!")
