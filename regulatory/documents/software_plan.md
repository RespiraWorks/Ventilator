{% extends "base/software_plan.md" %}

{% block project_details %}
## Development Standards

TODO: insert Development standards. Language, coding guidelines, etc..

[[:This section fulfills 5.1.4.a]]

## Development Methods

TODO: The project lead should keep an up-to-date list of development methods here (e.g., Test Driven Development) if relevant.

[[:This section fulfills 5.1.4.b]]

## Development Tools

TODO: The project lead should keep an up-to-date list of development tools here, such as linters and versions.

To the extent possible, checking against these standards should be performed in an automated fashion (e.g., using a linter which is run on a git-commit hook) [[:5.1.4]].

[[:This section fulfills 5.1.4.c]]

## Testing Plan

TODO: Write out a testing plan for {{ system.project_name }}.

This plan should include a pass/fail criteria for the entire test suite.  E.g., you require that all unit tests pass and that all integration tests pass or the cause of the failure is understood and justified [[62304:5.7.1.a]]

[[62304:5.5.2]]

{% endblock %}

{% block software_archival_task %}
TODO: Write out the task that should be followed in order to archive the software system release.  This will vary from project to project.  Here are some exmples:

- If the output of the build process is a binary, then the binary should be saved somewhere.

The purpose of the archive is to provide a means to re-test problems which may occur in an old version of the software.

{% endblock %}
