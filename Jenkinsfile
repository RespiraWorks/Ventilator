// Copyright 2023, RespiraWorks
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
                pwd
                """
            }
        }

        stage('Pre-commit') {
            steps {
                sh """
                ./jenkins/go.sh
                """
            }
        }
    }
}
