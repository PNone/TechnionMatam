#!/bin/bash
if [[ ! -z "$(git status --porcelain -uno | grep -v -e 'cmake-build-debug' -e '.idea' ) ]]; then
    echo "Cannot clean git while repo files were changed but not comitted" >&2;
    exit -1;
fi
read -p "Enter branch to implement fixes to [main]: " branch
branch=${branch:-main}
if [ ! -f ./.gitignore ]; then
    echo ".gitignore missing, creating and commiting the file"
    echo "cmake-build-debug/" > ./.gitignore
    echo ".idea/" >> ./.gitignore
    echo "mtm_blockchain" >> ./.gitignore
    echo "**/*.out" >> ./.gitignore
    git add ./.gitignore
    git commit -m "Add gitignore"
    git push
fi
git rm --cached -r ./cmake-build-debug/
git commit -m "Remove cmake-build-debug from git index"
git rm --cached -r ./.idea/
git commit -m "Remove idea folder from git index"
git rm --cached **/*.out
git commit -m "Remove tests' output from git index"
git rm --cached mtm_blockchain
git commit -m "Remove compiled executable from git index"
git push
