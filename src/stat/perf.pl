#!/usr/bin/perl

while (<>) {
    @line = split;
    print "$line[2] $line[4]\n" if $line[2]!=0;

}
