---
id: RELEASE-001
revision: 1
title: Release History
---

# Purpose

The purpose of this document is to list the change requests that were implemented within the current release.  It also includes approval of the change requests and the verification of the implemented changes.  Finally, it lists the problem reports that were addressed in the release as well as any outstanding problem reports (i.e., known anomalies).

# Scope

The scope of this document is the software system within the {{ system.project_name }} product.  It includes changes made within {{ system.release_id }}.

#  History

This section provides a summarized history of software revisions generated during the course of product development. 

{% for version in versions | reverse %}
## {{ system.project_name }} {{ version.release_id }} ({% if version.date %}{{ version.date }}{% else %}in progress{% endif %})
{% for change in version.changes or [] %}
- {{ change }}
{%- endfor %}
{%- endfor %}

Testing was performed against TODO: add version.

# Change Requests

This section includes a list of change requests and their associated changes, which were implemented for Release {{ system.release_id }} of {{ system.project_name }}.

{% for cr in history.change_requests|rejectattr('is_problem_report') %}
## {{ cr.title }}

**Identifier:** {% if cr.url is defined %}[{{ cr.id }}]({{ cr.url }}){% else %}{{ cr.id }}{% endif %}
{% if cr.content is defined and cr.content %}
**Description:**

{{ cr.content }}
{% endif %}
{% for c in cr.change_ids|join_to(history.changes) %}
**Implemented Change {% if c.url is defined %}[{{ c.id }}]({{ c.url }}){% else %}{{ c.id }}{% endif %}:**

Implemented by {{ c.authors[0].name }}
{%- if c.approvals %}, verified by {{ c.approvals[-1].reviewer.name }}{% endif %}.
{% if c.content is defined %}
{{ c.content }}
{% endif %}
{% endfor %}
{% endfor %}

# Problem Reports

This section includes a list of problem reports which were addressed for release {{ system.release_id }} of {{ system.project_name }}.

{% for cr in history.change_requests|selectattr('is_problem_report')|selectattr('change_ids') %}
## {{ cr.title }}

**Identifier:** {% if cr.url is defined %}[{{ cr.id }}]({{ cr.url }}){% else %}{{ cr.id }}{% endif %}

{# problem reports require a description, unlike normal change requests #}
**Description:**

{{ cr.content }}
{% for c in cr.change_ids|join_to(history.changes) %}
**Implemented Change {% if c.url is defined %}[{{ c.id }}]({{ c.url }}){% else %}{{ c.id }}{% endif %}:**

Implemented by {{ c.authors[0].name }}
{%- if c.approvals %}, verified by {{ c.approvals[-1].reviewer.name }}{% endif %}.
{% if c.content is defined %}
{{ c.content }}
{% endif %}
{% endfor %}
{% endfor %}

# Known Anomalies

This section includes a list of outstanding problem reports (i.e., known anomalies).  Each problem report should include the rationale why no changes were required.

{% for cr in history.change_requests|selectattr('is_problem_report')|rejectattr('change_ids') %}
## {{ cr.title }}

**Identifier:** {% if cr.url is defined %}[{{ cr.id }}]({{ cr.url }}){% else %}{{ cr.id }}{% endif %}

{# problem reports require a description #}
**Description:**

{{ cr.content }}
{% endfor %}
