{% extends "base/test_record.md" %}

{% block verification %}
# Verification

I, Full Developer Name, verify that the results recorded here are complete and accurate.

Tests were performed on March, 7 2030.

The tests meet our specified pass fail criteria of XYZ (see Test Plan section of the Software Plan).
{% endblock %}

{% block test_environment %}
# Test Environment

Describe the test environment.  This should include all of the information necessary for someone to reproduce the tests.  For example, it could be wise to include a list of all the environment variables, installed system packages and versions, the git hash, hardware used, etc.  It should also include any relevant testing tools.
{% endblock %}

{% block test_results %}
# Test Results
List of all the tests, split into sections by type (e.g., unit, integration, and manual).

List the pass/fail status.  Justify if tests fail why it is ok.

List any problems that were found during testing, and, if relevant, the problem report ids.

This must include the date and who performed the tests.  It also must assert that our pass/fail criteria listed in the Test Plan section of the Software Plan was met.

Finally, this should trace each test to 1 or more requirements, and should also verify that all requirements are covered (if they are not, we should add tests).

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

{% endblock %}
