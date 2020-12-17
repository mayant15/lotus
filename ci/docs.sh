#!/usr/bin/env bash
set -e

## SETUP
#####################################################################

# Git
GH_REPO_NAME="lotus"
GH_REPO_REF="github.com/studiocentauri/lotus.git"

git config --global push.default simple
git config user.name "kaka"
git config user.email "kaka@studiocentauri.in"

# Docs
OUTPUT_DIR="out"
DOXY_OUT=$OUTPUT_DIR/doxygen
CMAKE_SCRIPT="configure.cmake"


## BUILD
# Configure the Doxyfile with cmake and build
#####################################################################

cd docs
cmake -P $CMAKE_SCRIPT

echo "-- Generating docs"

# Generate doxygen
echo "-- Doxygen"
doxygen $DOXY_OUT/Doxyfile

# Generate sphinx
echo "-- Sphinx"
sphinx-build -b html -Dbreathe_projects.lotus=$DOXY_OUT/xml . $OUTPUT_DIR

echo "" > $OUTPUT_DIR/.nojekyll

## DEPLOY
# Deploy to GitHub https://gist.github.com/vidavidorra/548ffbcdae99d752da02
#####################################################################

echo "-- Deploying"
git clone https://git@$GH_REPO_REF
rm -rf $GH_REPO_NAME/*
cp -r $OUTPUT_DIR/** $GH_REPO_NAME

# Upload
cd $GH_REPO_NAME
git add --all
git commit -m "Update to ${CI_COMMIT_SHORT_SHA}"
git push --force "https://${GH_REPO_TOKEN}@${GH_REPO_REF}" > /dev/null 2>&1
