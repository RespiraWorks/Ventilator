# Jenkins CI

These are scripts run by the Jenkins server to run CI jobs for `pre-commit` `software/common`, `software/controller`, `software/gui`, and `software/utils`. See the Jenkinsfile at the root directory for pipeline details.

To manually run these tests, run
```
./do_docker_test.sh [precommit/common/controller/gui/utils]
```

TODO: gui and utils not yet implemented
