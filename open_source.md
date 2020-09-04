# RespiraWorks Open Source Philosophy

RespraWorks is an [Apache 2-licensed](https://opensource.org/licenses/Apache-2.0) open-source project (both hardware
and software), developed in the open.

A number of open-source veterans work on the project, and we take openness seriously, both in our software license and
in the way we work as a project.

## Choice of License (Permissive vs Copyleft)

For us, the choice of license came down to permissive vs copyleft. We chose a permissive license, and narrowed that down
to Apache 2.

A copyleft license like GPL requires that if you change then distribute the software, you have to make the changed
source code available.  This sounds good, and in many ways it is! But at the risk of taking a position in a religious
war, we believe that the interests specifically of COVID-19 are better served by software developed under a more
permissive license.

We have found that scrupulous organizations are hesitant to use copyleft (specifically GPL'ed) code.  It's easy to
accidentally link proprietary code with GPL'ed code, and if that ever escapes into the wild, all of your proprietary
code is now covered under the GPL and must be released as open-source.  In our experience, many organizations find this
to be an unacceptable risk and simply won't work on GPL'ed projects, even if they have every intention of upstreaming
all of their changes.

A permissive license doesn't have this problem, because you can take the code and do pretty much whatever you want with
it.  You don't have to share your changes under the same license.

The perceived risk of this is that an organization might take the open-source code, make many changes to it, and not
share those changes with the community - thus harming the overall project.

In our experience, the risk of this is low.  The value of an open-source project is not just in its source code, but
also in the community: the people who understand the code and can help.  If you make a closed-source fork of the code,
the community necessarily can't help, even if they wanted.  Moreover, maintaining a closed-source fork of a project is
hard, because as the underlying project changes, you have to carefully merge those changes back into your fork.  These
changes can be disruptive if they're made without any consideration of the fork.

Ultimately, we decided to use a permissive license because we want people to use our work in service of humanity.  We
hope that if they make substantial changes they will donate their work back to the upstream community, and we think it's
in their commercial interests to do so, because we expect they will want to interact meaningfully with the community and
because we expect maintaining a fork will be challenging.  But the bottom line for us is, if a third party doesn't want
to contribute their changes back to the main project, we would rather that they build ventilators than they don't build
ventilators.

Having chosen to use a permissive license, Apache 2 was an easy choice.  It's similar to MIT and BSD in that it doesn't
place major restrictions on what you can do with the code, but it adds some protection against malicious use of patents.
Specifically, if you own a patent and contribute code to the project, you can't then sue the project for infringing on
that patent.

## An Open Community

It's possible to run an open-source project where all development happens behind the scenes.  Occasionally, a new
version appears online, with huge and unexplained changes compared to the previous version.

We strive for the opposite of this.  In our project, we strive for all development to happen in a forum accessible to
all.  Anyone who upholds our community standards is welcome to participate, and indeed we have participants from around
the world.

In many ways we think this kind of openness is more important than the choice of open-source license, or even whether we
have an open-source license.  A closed community may be able to distribute nominally open-source software, but
fundamentally it can't grow beyond its bounds.
