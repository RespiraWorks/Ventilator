# Requirements

We formally manage our product requirements in
[Valispace](https://covent-19.valispace.com/specifications/requirements)

For access and guide on using Valispace efficiently, see the [Using Valispace](using_valispace.md) page.

Latest exports:
* [PDF format](system-reqs.pdf)
* [XLSX format](system-reqs.xlsx)
* **#TODO:** export to `json`

## Managing requirements

This is a living document, the entry point for learning how we manage requirements and how to help with that. If you
learn something useful, or if you think this page could be structured better, **please edit this page**.

As usual, where appropriate, **cross-link with other resources**: e.g. Slack threads where a decision was made (or names
of people who made a decision), Github issues tracking certain work etc.

## Are the requirements up to date?

Not yet, but we're trying to get there: many of them need clarification, many are missing, some may be vague or
redundant. If something looks off, it probably is - please do your part to clarify it and edit the requirement - see
below how.

## Is everything in Valispace?

As of May 19, system and software requirements have been transferred from various spreadsheets on Google drive.
The deprecated sources of this transfer are available for reference in
[this folder](https://drive.google.com/drive/folders/10EUN9lYFaMPZbI094FU3wP_rDw2_dcRc?usp=sharing).

**TODO:** Have electrical / PCB requirements been transferred?

Some additional artifacts are highly related to requirements or should be cross-linked with them, but are currently not
managed in Valispace:
* **Failure Modes and Effects Analysis (FMEA)** - currently managed in
[this spreadsheet](https://docs.google.com/spreadsheets/d/1qlTV5HqxnhlJXuhbSsEIfU-YwnN6PzgASv9_2dc5-cM/edit#gid=929514620)
  * **TODO:** How do we manage cross-linking between FMEA and the requirements?
  * **TODO:** We should have a wiki page explaining the FMEA
* **Validation logs** - when we have a formal validation effort spun up, we will need to be able to trace every
requirement to logs of tests showing that the requirement is met in a specific version of the product.
* **TODO:** Anything else?

## Where did they come from?

The **system requirements** come from a variety of sources: ISO standards (**TODO:** which ones?), ventilator manuals
(**TODO:** which ones?).

The **software requirements** are a result of people from the Software team analyzing the system requirements from their
perspective and mapping them onto the software architecture
(**TODO:** we should have a page about software architecture).

**TODO:** Where did the other ones come from?

## How will we know that they are correct and complete?

**TODO:** How?

## How will I know when a requirement changes?

**TODO:** How?

## I have a general question about managing requirements

Ask it on `#requirements`. (**TODO:** Are there more detailed places where one should go with more specific questions?
Eg. hardware requirements, legal requirements, what to do if you need to talk to a doctor to clarify something?)

## I need to clarify an existing requirement

Use the Valispace "discussion" feature on the respective requirement.
**TODO:** Describe the feature in more detail, e.g. will I get a notification if somebody responds? Will I get a
notification if somebody asks a question about a requirement I added?

## I need to add a new requirement

If you found out that the system needs to have some important requirement that is currently not tracked, please add it
to Valispace and link it with other related requirements. For example, if you looked at the requirement "must have a
BATTERY LOW alarm" and found that there is no requirement to have a battery sensor, go ahead and add a requirement to
have a battery sensor.

If you're adding a new hardware requirement, ping David Gerson on Slack and let him know.

## Which requirements are higher-priority?

**TODO:** This ideally should link to a well-prioritized "milestones" page

# Cross-cutting TODOs

In addition to the explicitly listed questions on each requirement, a few recurring themes implicitly apply to almost
every requirement. We need to conduct several large-scale requirement cleanups.

* For all requirements we need to know which modes are they relevant to
  * **TODO:** How will we represent this in Valispace?
  * **TODO:** Classify all requirements by modes where appropriate - **should file a Github issue to track this**

* Many requirements talk about displaying a value, or a value being within certain bounds. It is often unclear whether
the value is an instant sensor reading, a computation from the most recent breath, a time series thereof, or an average
over some window (time-delimited or breath-delimited)
  * **TODO:** Audit all such requirements and clarify them -  **should file a Github issue to track this**

* Many values can be "commanded" (i.e. the device is trying to achieve a specific value) and "measured" (i.e. we can
measure what value has actually been achieved). In some requirements it is unclear which modality it is talking about.
  * **TODO:** Apply either "commanded" or "measured" to all magnitudes in all requirements - **should file a Github issue to
  track this**

* Requirements for device performance (how quickly and how precisely the device should achieve commanded values of
different parameters) are incomplete. There is a catch-all of 10% (**TODO:** insert ID for this requirement).
  * **TODO:** Identify such requirements for all necessary parameters - **should file a Github issue to track this**
