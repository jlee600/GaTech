#!/bin/bash

imageName="gtcs2110/proj03-fall24:latest"

if ! docker -v >/dev/null; then
  >&2 echo "ERROR: Please install Docker before running this script. Refer to the CS 2110 Docker Guide."
  exit 1
fi

if ! docker container ls >/dev/null; then
  >&2 echo "ERROR: Docker is not currently running. Please start Docker before running this script."
  exit 1
fi

echo "Attempting to pull down most recent image of $imageName..."

if ! docker pull "$imageName"; then
  >&2 echo "WARNING: Unable to pull down the most recent image of $imageName"
fi

if command -v docker-machine &> /dev/null; then
  # We're on legacy Docker Toolbox
  # pwd -W doesn't work with Docker Toolbox
  # Extra '/' fixes some mounting issues
  curDir="/$(pwd)"
else
  # pwd -W should correct path incompatibilites on Windows for Docker Desktop users
  curDir="/$(pwd -W 2>/dev/null || pwd)"
fi

docker run --rm -i -v "$curDir":/autograder/submission/ "$imageName" "//autograder/run_local" "$1"

if [ $? -ne 125 ]; then
  OS_NAME=$(uname)
  if [ "$OS_NAME" = "Darwin" ]; then
    open ./tests/report.html
  elif [ "$OS_NAME" = "Linux" ]; then
    xdg-open "tests/report.html" &
  fi
fi
