# Manufacturing design

This section of the repository describes the physical design of the ventilator and the process of manufacturing the
components and fully assembled units.

If you are a member of the team and intend to build your own prototype, please read the
[purchasing guidelines](#part-purchasing-guidelines) first.

Broadly there are two types of ventilator builds we are currently testing with:
- open table-top prototypes for testing components and various pneumatic configurations, kindly referred to as "pizza build"
- "enclosed build", more like the final product; more expensive to make and slower to iterate on

| Pizza build  |  Enclosed build   |
|:---------------------:|:--------------------:|
![](pizza_build/images/assembled.jpg) |![](enclosed_build/images/India_build_yellow.jpeg) |

## Sub-assemblies

Components and sub-assemblies have firstly been tested and documented in the context of the pizza build. They should
generally be similar for the enclosed build, but documentation might be lagging.

One of the foundational principles of the ventilator design was to avoid, where possible, custom components. However,
for several parts, it was unavoidable to design a new component if price, availability, and functional performance
requirements could not be achieved with already-available components. Some of the following contain components that
have been custom designed for this application. The manufacturing process for the prototypes is discussed, as well as
plans and features for production at larger volumes, with an approximate scale-point of 1,000 units. For smaller orders
of less than 100 units production, some of the prototype production methods may prove more cost- and time-effective.

For each component, the goal is to document:
* the reasoning for the design
* summary of design history
* files or links to original CAD, derived step files and 3d-printer files with scaffolding
* prototype fabrication process and common problems to watch out for
* plans for fabrication at scale
* link to characterization tests and data
* link to software that interfaces with component
* link to any validation tests that are available to evaluate individual assemblies

Some of the above may not be done for each component. Feel free to help us fill this out.

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
