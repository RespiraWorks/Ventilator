# Manufacturing design

This section of the repository describes the physical design of the ventilator and the process of manufacturing the
components and fully assembled units.

Broadly there are two types of ventilator builds we are currently testing with:
- open table-top prototypes for testing components and various pneumatic configurations, kindly referred to as "pizza build"
- "enclosed build", more like the final product; more expensive to make and slower to iterate on

| Pizza build  |  Enclosed build   |
|:---------------------:|:--------------------:|
![](pizza_build/graphics/3_pizzas_small.jpg) |![](enclosed_build/graphics/india1.jpeg) |

If you are a member of the team and intend to build your own prototype, please read the
[purchasing guidelines](#part-purchasing-guidelines) first.

## Sub-assemblies

Components and sub-assemblies have firstly been tested and documented in the context of the pizza build. They should
generally be similar for the enclosed build, but documentation might be lagging.

For each component, the goal is to document:
* the reasoning for the design
* production process and common problems to watch out for
* link characterization tests and data
* link to software that interfaces with component
* link to any validation tests that are available to evaluate individual assemblies

Some of the above may not be done for each component. Feel free to help us fill this out.

The following document was produced for the Covent19 challenge. Contents therein should at some point be migrated
over into the repository as plain text:
[Production methods for custom components (PDF)](assets/covent-june-2020-production-methods-for-custom-components.pdf)

### Components

* [Brain build](brain) - constitutes just the computing components
* [Blower assembly](blower) - this provides pressurized air
* [Venturi flow sensors](venturi) - in-house design of affordable flow sensors
* [Pinch valve](pinch_valve) - in-house design of affordable valve for inhale and exhale control
* [Air filter assembly](filter_holder) - air filters for keeping patient safe and system clean
* **TODO:** oxygen limb of pneumatic circuit

### Characterization Testing

For details on characterization tests, see the [Characterization Test Plan](characterization-test-plan.md).

## Full builds

[**Pizza build**](pizza_build)
A fundamentally functional ventilator that is an upgrade that includes sensors, drivers and pneumatics.
It is not enclosed like the final product and is meant to be operated in an open layout on your table, like a pizza.

[**Enclosed build**](enclosed_build)
This is our work-in-progress towards a "beta" version of the ventilator, which should be rather close to the final
product.

Additionally, the following document was produced for the Covent19 challenge. It encompasses some aspects of both the
pizza and the enclosed build. **TODO:** Contents therein should at some point be migrated over into the repository as
plain text: [Covent19 assembly instructions](assets/covent-june-2020-assembly-instructions.pdf)

## Test equipment

To test a prototype you will need some additional components that are not strictly part of the ventilator itself, such
as test lungs.

There is a page dedicated to [Testing](../quality-assurance/testing) under the quality assurance section of the repository.

## Part Purchasing Guidelines

If you are a member of the RespiraWorks team, please don’t buy anything using personal funds (we need to document everything in specific ways to meet the requirements of our grant funding).

Before purchasing any parts, **ask** team members (the [#dev-hardware](https://respiraworks.slack.com/archives/C012UTERXD5) RespiraWorks Slack channel is a good place to start) if they have any spare parts floating around. There are a lot of extras, so you may be able to get your hands on parts without ordering anything.

If you do need to purchase parts, follow the process below. Note: This process is subject to change as we work out any issues. Message Curtis Kline with any questions.

1. If you need to buy something, put the details into a post in [#purchasing-sourcing](https://respiraworks.slack.com/archives/C011FD2TEQM) (RespiraWorks Slack channel) and tag the [@purchasing group](https://respiraworks.slack.com/admin/user_groups).
2. One of the Purchasing team members will source the items and confirm with you before placing the order (unless it’s obvious).
3. Everything will be purchased using a RespiraWorks credit card whenever possible.
4. The Purchasing team member will reply to your message in [#purchasing-sourcing](https://respiraworks.slack.com/archives/C011FD2TEQM) with tracking info when available.
5. in the rare case that you do buy something urgent with your own payment methods, submit your receipts to #accounting-reimburse.
