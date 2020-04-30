---
category: TESTREC
id: TESTREC-001
revision: 1
title: Software Test Record
manufacturer_name: MANUFACTURER
---

# 1 Purpose

The purpose of this document is to record the status of the software tests run for RespiraWorks Ventillator Software.

# 2 Scope

The scope of this document is the software system within the RespiraWorks Ventillator Software product.


# 3 Verification

I, Full Developer Name, verify that the results recorded here are complete and accurate.

Tests were performed on March, 7 2030.

The tests meet our specified pass fail criteria of XYZ (see Test Plan section of the Software Plan).


# 4 Test Environment

Describe the test environment.  This should include all of the information necessary for someone to reproduce the tests.  For example, it could be wise to include a list of all the environment variables, installed system packages and versions, the git hash, hardware used, etc.  It should also include any relevant testing tools.


# 5 Test Results
List of all the tests, split into sections by type (e.g., unit, integration, and manual).

List the pass/fail status.  Justify if tests fail why it is ok.

List any problems that were found during testing, and, if relevant, the problem report ids.

This must include the date and who performed the tests.  It also must assert that our pass/fail criteria listed in the Test Plan section of the Software Plan was met.

Finally, this should trace each test to 1 or more requirements, and should also verify that all requirements are covered (if they are not, we should add tests).

## 5.1 Unit Tests

| Test Name | Test Status | Requirement IDs | Notes |
| --- | --- | --- | --- |
| TestClass.TestName1 | pass | ['SR-12'] |  |
| TestClass.TestName2 | fail | ['SR-12'] | It is okay that this test failed because of XYZ |


## 5.2 Integration Tests
| Test Name | Test Status | Requirement IDs | Notes |
| --- | --- | --- | --- |
| TestClass.TestName100 | pass | ['SR-13', 'SR-14'] |  |
| TestClass.TestName200 | fail | ['SR-17'] | It is okay that this test failed because of XYZ |
| TestClass.TestName300 | pass | ['SR-17'] |  |


## 5.3 Manual Tests
| Test Name | Step | Test Status | Requirement IDs | Notes |
| --- | --- | --- | --- | --- |
| ManualTest1 | 5 | pass | ['SR-18'] |  |
| ManualTest1 | 6 | fail | ['SR-19'] | Allowed because of XYZ |
| ManualTest2 | 1 | pass | ['SR-11', 'SR-12'] |  |
