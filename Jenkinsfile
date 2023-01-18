pipeline {

    agent any

    options {
        buildDiscarder logRotator(
                    artifactDaysToKeepStr: '',
                    artifactNumToKeepStr: '5',
                    daysToKeepStr: '16',
                    numToKeepStr: '10'
            )
    }

    stages {

        stage('Cleanup Workspace') {
            steps {
                cleanWs()
                sh """
                echo "Cleaned Up Workspace For Project"
                """
            }
        }

        stage('Code Checkout') {
            steps {
                checkout scm
            }
        }

        stage('Branch info') {
            steps {
                sh """
                echo "BRANCH_NAME = $BRANCH_NAME"
                git status
                ls -al
                ls -al manufacturing/images
                """
            }
        }

        stage('Pre-commit') {
            steps {
                sh """
                merge_base=$(git merge-base -a HEAD origin/master)
                changed_files=$(git diff --name-only $merge_base...HEAD)
                echo "Changed files since branched from origin/master: " $changed_files
                git reset --soft $merge_base
                pre-commit run --show-diff-on-failure --files $changed_files
                """
            }
        }

    }
}
