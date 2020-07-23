# This milestone is complete.

_Information below only for posterity._

# Objectives
* Provide a physical proof-of-concept for demonstration and review with medical experts
  * See **Demo script** section below.

* Provide a platform for continuous engineering experimentation
  * E.g. Edwin will want to play with the thing with a test lung. Ideally the software should be conductive to that, e.g. have the parameters adjustable via UI rather than baked in.

# Requirements

* Must have a minimal GUI (graphical user interface) that can display pressure trace, flow trace, volume trace
  * GUI is ideally our specialized QT GUI, but if that's not ready by demo time, we can use [this approach](https://github.com/RespiraWorks/PhysicalDesign/wiki/Plotting-and-recording-live-data-from-an-MCU)
    * *jlebar*: That approach won't work with our current protobuf-based protocol.  We can change our code to output something parseable by that tool if necessary, but my preference would be to do something that we don't have to throw away.  For example, Albertas put together a [proof of concept Python script](https://github.com/RespiraWorks/Ventilator/commit/330895ea391ee70cf156fb5bd8673434b3908846) that reads protobuf data from the Arduino.  We could use an approach like this to draw graphs in Python.

* GUI must obtain pressure, flow, volume data from the controller
  * [Per Ethan](https://respiraworks.slack.com/archives/C012H3G1GQ5/p1587857837002500): Flow/volume do not need to be right, just look right:
  * Ideally, use sensor readings with calibration at device startup and smoothing
  * But for alpha, OK to do calibration once on the demo device and hard-code calibration parameters; omit smoothing
  * Absolute worst case, use synthetic data for demo

* Controller must implement Pressure Control (aka Command Pressure) mode
  * Defined in [this presentation slide](https://docs.google.com/presentation/d/1DA1BQlCj8wNv60pGGzrldPfZBmr6WAH4WbrD4W7i43U/edit#slide=id.g739b1a32cc_0_12), prototyped in [pid.cpp](https://github.com/RespiraWorks/Ventilator/blob/fee155d5d4e8fa029b2e83e4753cfaeb15b9b365/controller/src/pid.cpp), correct implementation tracked [here](https://github.com/RespiraWorks/Ventilator/issues/17)
  * Within alpha, for demo, controller can implement this autonomously, i.e. with parameters hard-coded in the controller rather than obtained from the GUI.
  * However, getting mode parameters from GUI is a nice-to-have, per objective 2 above.

* Must not crash irrecoverably or self-destruct.

# Demo Script
This is a narrative description of how we expect the alpha test to go.  We will probably be running a subset of the tests described in Appendix B of the [MHRA Requirements](https://assets.publishing.service.gov.uk/government/uploads/system/uploads/attachment_data/file/879382/RMVS001_v4.pdf).

* Hook up RespiraWorks Ventilator to a test lung
* Turn on device and begin showing data on a screen, either on the GUI or a laptop
* Run a set of respiratory cycles at default, middle of the range values.
* Adjust each of PIP, PEEP, I:E, and RR one at a time, starting from default values, while recording data.
* Run some of the more extreme corner cases involving two or more of the above settings (i.e. High PIP, low PEEP, Low I:E, high RR, high resistance on test lung, high compliance on test lung), while recording data.
* Simulate common failure modes (leaky loop, patient obstruction, etc), while recording data.
* Simulate patient inspiratory effort, record data.
* End test

*TODO(2020-04-29)* Clarify with Edwin: What behavior do we expect SW to have in extreme cases / failure modes?  Similarly, what behavior do we expect when simulating patient effort?  Current understanding is that, nothing is needed from software.

* Note: For patient effort, failure modes, and corner cases, we don't except the alpha software to do anything except continue to drive the respiration cycle according to the set parameters.  We just want to record what these cases look like to our sensors.

# Excluded

The alpha milestone is part of the project's overall [requirements](https://github.com/RespiraWorks/Ventilator/wiki/Requirements). Anything mentioned there but not here, is excluded from alpha.

If you have further questions, ask on [#alpha-reqs](https://app.slack.com/client/T0102KRCH5M/C012H3G1GQ5/thread/C0100SF3N5T-1587858521.093800).

# Issue hotlists

Proceed with caution: these need cleanup.

- Controller: https://github.com/RespiraWorks/Ventilator/milestone/4
- GUI: https://github.com/RespiraWorks/Ventilator/milestone/1
