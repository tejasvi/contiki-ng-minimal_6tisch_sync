#About this repository
This repository contains the code used for the experiments of the paper:
```
A. Karalis, D. Zorbas and C. Douligeris, "Optimal Initial Synchronization Time in the Minimal 6TiSCH Configuration" in 
IEEE Access.
```

Since the experiments were run using the operating system [Contiki-NG](https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=&cad=rja&uact=8&ved=2ahUKEwjS3eL74pHtAhWEGuwKHXuoApIQFjAAegQIAhAC&url=https%3A%2F%2Fgithub.com%2Fcontiki-ng%2Fcontiki-ng&usg=AOvVaw0Sog-YZjOlUdSNxURbnU2K),
the core of the code is Contiki-NG. For the additional code of our experiments we created a subfolder named [`minimal_6tisch_sync`](https://github.com/akaralis/contiki-ng-minimal_6tisch_sync/tree/minimal_6tisch_sync/examples/minimal_6tisch_sync) 
in the folder [`examples`](https://github.com/akaralis/contiki-ng-minimal_6tisch_sync/tree/minimal_6tisch_sync/examples) of the Contiki-NG's source code tree. For more information about the experiments see at the [README](https://github.com/akaralis/contiki-ng-minimal_6tisch_sync/tree/minimal_6tisch_sync/examples/README.md)
file contained in
this subfolder.

<!----
<img src="https://github.com/contiki-ng/contiki-ng.github.io/blob/master/images/logo/Contiki_logo_2RGB.png" alt="Logo" width="256">

# Contiki-NG: The OS for Next Generation IoT Devices

[![Build Status](https://travis-ci.org/contiki-ng/contiki-ng.svg?branch=master)](https://travis-ci.org/contiki-ng/contiki-ng/branches)
[![Documentation Status](https://readthedocs.org/projects/contiki-ng/badge/?version=master)](https://contiki-ng.readthedocs.io/en/master/?badge=master)
[![license](https://img.shields.io/badge/license-3--clause%20bsd-brightgreen.svg)](https://github.com/contiki-ng/contiki-ng/blob/master/LICENSE.md)
[![Latest release](https://img.shields.io/github/release/contiki-ng/contiki-ng.svg)](https://github.com/contiki-ng/contiki-ng/releases/latest)
[![GitHub Release Date](https://img.shields.io/github/release-date/contiki-ng/contiki-ng.svg)](https://github.com/contiki-ng/contiki-ng/releases/latest)
[![Last commit](https://img.shields.io/github/last-commit/contiki-ng/contiki-ng.svg)](https://github.com/contiki-ng/contiki-ng/commit/HEAD)

Contiki-NG is an open-source, cross-platform operating system for Next-Generation IoT devices. It focuses on dependable (secure and reliable) low-power communication and standard protocols, such as IPv6/6LoWPAN, 6TiSCH, RPL, and CoAP. Contiki-NG comes with extensive documentation, tutorials, a roadmap, release cycle, and well-defined development flow for smooth integration of community contributions.

Unless explicitly stated otherwise, Contiki-NG sources are distributed under
the terms of the [3-clause BSD license](LICENSE.md). This license gives
everyone the right to use and distribute the code, either in binary or
source code format, as long as the copyright license is retained in
the source code.

Contiki-NG started as a fork of the Contiki OS and retains some of its original features.

Find out more:

* GitHub repository: https://github.com/contiki-ng/contiki-ng
* Documentation: https://github.com/contiki-ng/contiki-ng/wiki
* Web site: http://contiki-ng.org
* Nightly testbed runs: https://contiki-ng.github.io/testbed

Engage with the community:

* Gitter: https://gitter.im/contiki-ng
* Twitter: https://twitter.com/contiki_ng
---->