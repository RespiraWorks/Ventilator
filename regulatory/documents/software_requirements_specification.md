---
id: SRS-001
revision: 1
title: Software Requirements Specification
---

# Purpose

The purpose of this document is to list the requirements that describe *what* the {{ system.project_name }} {{ system.release_id }} software must fulfill.

This document is meant to be read and agreed-upon by the project owners and by software developers during design and construction.

{% if not system.is_software_only_device %}
The document also provides traceability between system requirements and software requirements.
{% endif %}

# Scope

The scope of this SRS applies in its entirety to the {{ system.project_name }} {{ system.release_id }} product.

# Definitions

The **Food and Drug Administration (FDA)** is a United State government agency responsible for protecting the public health by ensuring the safety, efficacy, and security of human and veterinary drugs, biological products, and medical devicese

The **Health Insurance Portability and Accountability Act** (HIPAA) is a United States law designed to provide privacy standards to protect patients' medical records and other health information provided to health plans, doctors, hospitals and other healthcare providers.

**Protected Health Information** (PHI) means individually identifiable information that is created by {{ system.project_name }} and relates to the past, present, or future physical or mental health or condition of any individual, the provision of health care to an individual, or the past, present, or future payment for the provision of health care to an individual.

**UI** is an acronym for user interface.

# Stakeholders

Stakeholders are anyone who is affected by or interested in the project.  This section of the document should list out the project stakeholders and describe them and why they are interested in the project.  Different stakeholders have different requirements for the project, so it is useful to enumerate all of them so that no important requirements are missed.  As with every step in the requirements process, the goal is to be optimally valuable.  It is easy to go overboard and list out 30 or 40 stakeholders, but this is usually not optimally valuable on smaller projects.  For example, a worker at an electronics recycling plant is a stakeholder, but startups and small companies don't have time to consider their requirements for a project.  A few common stakeholders are listed below for convenience.

## Project Owner

The project owner wants the project to be profitable.

## Project Sponsor

The project sponsor is the primary decision maker who acts on behalf of the owner.

## Customer

The purchaser wants the device to produce value for their organization.

- Where are the customers geographically?
- How many customers are there?
- Is the customer also the user, or are they buying on behalf of users within a larger organization?

## Patient

The {{ system.project_name }} is ultimately used to help patients.

## User

The user wants the device to make their job easier.  There may be several different types of users, in which case it is worth adding more sections for each type.

- What part of their work do they want to improve using the project?
- What is the background of the user?
- What is their education level?
- What is their age?
- What motivates them at their work?

## Advanced User

The advanced user is typically part of a smaller group of users, who uses the software more frequently than normal users, and thus has more requirements.

## Service Engineer

The service engineer wants the software to capture information so that it is easy to detect and debug problems.

## Regulator

The FDA wants to ensure that the {{ system.project_name }} device is safe, and they want to be able to do so as quickly and efficiently as possible.

The Human Health Services (HHS), who are responsible for enforcing HIPAA, want to ensure that private health information is not leaked out to the public.

Regulators in other jurisdictions will have similar requirements.

# Use Cases

## Problem X

Brief description.

## Problem Y

Brief description.

# Requirement Details
{% for requirement in requirements.requirements %}
## {{ requirement.title }}

*Requirement ID:* {{ requirement.id }}

{{ requirement.description }}
{% endfor %}

# Traceability Tables
{% if system.is_software_only_device %}
## Software Requirements Table

| ID | Title |
| --- | --- |
{%- for requirement in requirements.requirements %}
| {{ requirement.id }} | {{ requirement.title }} |
{%- endfor %}
{% else %}
## Software Requirements Table

| Soft. Req. ID | System Req. IDs | Title |
| --- | --- | --- |
{%- for requirement in requirements.requirements %}
| {{ requirement.id }} | {{ requirement.system_requirements|join(', ') }} | {{ requirement.title }} |
{%- endfor %}

## System Requirements Mapping

| System Req. ID | Soft. Req. IDs |
| --- | --- |
{%- for system_requirement_id, software_requirement_ids in requirements.requirements|invert_dependencies('id', 'system_requirements') %}
| {{ system_requirement_id }} | {{ software_requirement_ids|sort|join(', ') }} |
{%- endfor %}
{%- endif %}
