#!/usr/bin/env python3

# Startup script for the LC3 autograder.
#
# This script is automatically run by `./grade.sh` or `.\grade.bat`. 
# Use those if you don't have Python.
#
# If you do have Python installed, you can install all the dependencies for the autograder with:
#   python3 -m pip install "lc3-ensemble-test[std]" (macOS/Linux)
#   py -m pip install "lc3-ensemble-test[std]" (Windows)
#
# Once completed, you can call this script with:
#   python3 autograder.py (macOS/Linux)
#   py autograder.py (Windows)

from pathlib import Path
import pytest
import sys
import webbrowser
import os

if __name__ == "__main__":
    OUTPUT_PATH = "report.html"

    if os.path.exists(OUTPUT_PATH):
        os.remove(OUTPUT_PATH)

    # If not running in Docker, override file resolution to the directory with the ASM files
    # (instead of the test/ directory).
    if not os.getenv("RUNNING_DOCKER"):
        os.environ["TEST_ROOT_DIR"] = str(Path(__file__).parents[1])
    
    retcode = pytest.main(["--html", OUTPUT_PATH, "--self-contained-html", *sys.argv[1:]])

    # Automatically opens report page in web browser
    # Unfortunately has to open a new browser window every single time.
    if not os.getenv("RUNNING_DOCKER"):
        path = Path(OUTPUT_PATH).resolve()
        if path.exists():
            webbrowser.open(path.as_uri())

    exit(retcode)
