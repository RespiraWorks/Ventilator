---
id: TESTREC-001
title: Software Test Record
---

# Purpose

The purpose of this document is to record the status of the software tests run for {{ system.project_name }}.

# Scope

The scope of this document is the software system within the {{ system.project_name }} product.

# Verification

I, FULL DEVELOPER NAME, verify that the results recorded here are complete and accurate.

Tests were performed on DATE TESTS COMPLETED.

The tests meet our specified pass fail criteria (see Test Plan section of the Software Plan).

# Test Environment

TODO: Describe the test environment.  This section should include all of the information necessary for someone to reproduce the tests.  For example, it could be wise to include a list of all the environment variables, installed system packages and versions, the git hash, hardware used, etc.  It should also include any relevant testing tools.

# Test Results

TODO: List of all the tests, split into sections by type.  You can use the three subsections below as a starting point.

TODO: List any problems that were found during testing, and, if relevant, the problem report ids.

It is ok if some tests do not trace to any particular requirements, however all requirements must be covered by some tests (if they are not, add tests).

## Unit Tests

| Test Name | Test Status | Requirement IDs | Notes |
| --- | --- | --- | --- |
{% for test_name in unit_test_record -%}
| {{ test_name }} | {{ unit_test_record[test_name].result }} | {{ unit_test_record[test_name].req_ids }} | {% if unit_test_record[test_name].note is defined %}{{ unit_test_record[test_name].note }}{% endif %} |
{% endfor %}

## Integration Tests

| Test Name | Test Status | Requirement IDs | Notes |
| --- | --- | --- | --- |
{% for test_name in integration_test_record -%}
| {{ test_name }} | {{ integration_test_record[test_name].result }} | {{ integration_test_record[test_name].req_ids }} | {% if integration_test_record[test_name].note is defined %}{{ integration_test_record[test_name].note }}{% endif %} |
{% endfor %}

## Manual Tests

| Test Name | Step | Test Status | Requirement IDs | Notes |
| --- | --- | --- | --- | --- |
{% for manual_test in manual_tests -%}
{% for manual_step in manual_test.steps -%}
| {{ manual_test.name }} | {{ manual_step.step }} | {{ manual_step.result }} | {{ manual_step.req_ids }} | {% if manual_step.note is defined %}{{ manual_step.note }}{% endif %} |
{% endfor -%}
{% endfor -%}
