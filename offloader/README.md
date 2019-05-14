# Offloader #

Welcome to this tutorial series where you will build a *CPU offloader* on an FPGA!

## Scope of project ##

So, a CPU offloader is essentially a compute engine. It functions somewhat like
an extra CPU core, but can perform dedicated operations much faster, by
utilizing the special features of the FPGA.

The CPU offloader will run on the Nexys4DDR FPGA board, and will communicate
with the main PC via the Ethernet port. The way this works is that the main PC
sends a UDP packet to the Nexys4DDR with a computation command, and some time
later the Nexys4DDR sends one or more UDP packets back to the main PC with the
result of the computation.

The reason for using Ethernet is that the data transfer rate of 100 Mbit/s is
pretty fast, compared to using the USB port. And the reason for using UDP
packets is that it is a compromise between what is easy to implement in the
FPGA and what is easy to implement on the host PC. I could alternatively have
used raw Ethernet packets, but that would probably require root access to the
network driver on the PC, On the other hand, using TCP would require a lot of
design work on the FPGA. Therefore, UDP was chosen as a compromise.  This
choice does however require building a network stack on the FPGA, in particular
responding to e.g. ARP packets. More on that later.

Apart from being able to offload the CPU of the main PC, this project will be
built step by step, documenting each step as we go along, and thereby serve as
a dual function of a tutorial in VHDL programming on an FPGA, i.e. digital
systems design.

## Overall FPGA design ##

The main idea as mentioned is to utilize the Ethernet port on the Nexys4DDR
board.  The actual computations will be performed directly in the FPGA, and I
have chosen to write the Ethernet communication protocols like that too. An
alternative is to build a small SoC, i.e. have a CPU running on the FPGA that
performs the network communication. However, since computation speed is
important (that is afterall why we are offloading in the first place), it makes
sense to reduce network latency.

To help debugging, we will make heavy use of the VGA output of the Nexys4DDR.
This is not essential for the functionality itself, but gives a pretty cool
view of what is going on inside the FPGA.

## Overview of series ##

In the first episode we'll be installing the necessary development tools,
setting up the project, and copying the VGA engine from the
[DYOC](https://github.com/MJoergen/nexys4ddr/tree/master/dyoc) series.

In the next few episodes, we'll build a network processor, allowing the FPGA to
receive and send UDP packets.

## List of episodes: ##
### VGA ###
1.  [**"Hello World"**](Episodes/ep01_-_Hello_World). Here you will generate a
    checkerboard pattern on the VGA output.
2.  [**"Adding hexadecimal output to VGA"**](Episodes/ep04_-_Hexadecimal). Here we will
    implement a complete font and show data in hexadecimal format.

More to come soon...

## Prerequisites ##

### FPGA board ###

To get started you need an FPGA board. I'll be using 
[Nexys 4 DDR](https://reference.digilentinc.com/reference/programmable-logic/nexys-4-ddr/start)
(from Digilent), because it has an Ethernet port.

### FPGA toolchain (software) ###

The Nexys 4 DDR board uses a Xilinx FPGA, and the toolchain is called
[Vivado](https://www.xilinx.com/support/download.html).
Use the Webpack edition, because it is free to use.

## Recommended additional information ##

I will in this series assume you are at least a little familiar with logic
gates and digital electronics.

I will also assume you are comfortable in at least one other programming
language, e.g. C, C++, Java, or similar.

## About me ##

I'm currently working as a professional FPGA developer, but have previously
been teaching the subjects mathematics, physics, and programming in High School.
Before that I've worked for twelve years as a software developer.

As a child I used to program in assembler on the Commodore 64. This "heritage"
is reflected in some of the design choices for the present project, e.g.
choosing the 6502 processor.
