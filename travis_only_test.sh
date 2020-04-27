# Tests in this file only run on Travis, these tests have dependency on
# the Travis enviorment, not suitable to run locally.

# TODO: Disabled because $TRAVIS_BRANCH sometimes can be set incorrectly
# (https://github.com/RespiraWorks/VentilatorSoftware/issues/184)
# Pre-commit checks are only runing on changed files.
# changed_files=$(git diff --name-only $TRAVIS_BRANCH...HEAD)
# echo "Changed files: " $changed_files
# Make git think these changes are uncommitted
# git reset --soft $TRAVIS_BRANCH
# Run precommit only on the files changed in our branch since merge-base
# pre-commit run --show-diff-on-failure --files $changed_files
