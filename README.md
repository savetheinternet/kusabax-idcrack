## Requirements
1.	21GB disk space for rainbow table.
2.	OpenSSL library.

## Background
Currently, Kusaba X and a lot of Kusaba X ports use a very insecure algorithm for calculating "Poster IDs". That is, a semi-unique ID used as a means of identification. Unfortunately, the developers must have overlooked the major flaw in the algorithm when developing the function. The poster IDs are calculated by simply truncating an (unsalted) MD5 hash of the dotted decimal representation of the user's IP address to 6 characters. As there are 2<sup>32</sup> (before excluding multicast, private and reserved space) IPv4 addresses and only 2<sup>24</sup> possible IDs, it makes it possible (and incredibly easy) to narrow an ID down to ~256 IP addresses. The rest could possibly be eliminated using GeoIP or any other means.

Although it may seem unpractical because of the time and processing power required to test a hash against every single IP address, it could be done in just minutes with a large rainbow table.

In short, this simple program will turn a standard Kusaba X "Poster ID" into ~256 possible IPv4 addresses.

## Excluded Ranges
 - Class D and E addresses
 - 10.0.0.0/8
 - 127.0.0.0/8
 - 172.16.0.0/12
 - 192.168.0.0/16
 
After excluding multicast, private and reserved space, there are 3 706 584 831 addresses (~14% deduction).

## Usage
To compile, use `make`.

### Generation
To generate the rainbow table, use `./ids <database filename> --generate`. Depending on disk and CPU speeds, this may take a few minutes (or even hours), but only needs to be done once. This will produce a 21GB file.

### Search
To decode an ID, use `./ids <database> <poster id>`. All possible IPv4 addresses will be printed to standard output line by line. Again depending on CPU and disk speeds, this may take a few minutes.

