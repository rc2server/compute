#!/usr/bin/env perl

### Connects to compute engine assuming it is exposed on port 7714.
### Enter a query to execute it. For anything else,
### @file.json will send the json in file.

#use strict;
use warnings;

use lib 'lib';
use AnyEvent::Handle;
use AnyEvent::Socket;
use Devel::Hexdump 'xd';
use Cpanel::JSON::XS qw(decode_json encode_json);
use File::Slurp;

my $con_ready = AnyEvent->condvar;
my $handle;
my $connectStr = '{"msg":"open","argument":"","dbname":"rc2","wspaceId":100,"dbuser":"rc2","dbhost":"localhost","sessionRecId":463}';
tcp_connect "localhost", 7714, sub {
	my ($fh) = @_ or return $con_ready->send;
	
	$handle = new AnyEvent::Handle(
		fh => $fh,
		on_error => sub {
			print STDERR "got error\n";
			$_[0]->destroy;
		},
		on_eof => sub {
			print STDERR "got eof\n";
			$handle->destroy;
		},
		on_read => sub {
			shift->unshift_read ( chunk => 8, sub {
				my ($bit, $len) = unpack("NN", $_[1]);
				shift->unshift_read (chunk => $len, sub {
					print $_[1] . "\n";
				});
			});
		});
		
	my $read_watcher; $read_watcher = AnyEvent->io (
		fh => $fh,
		poll => "r",
		cb => sub {
			print STDERR "connection open\n";
		});

	&sendMessage($connectStr);
#	my $data = pack("NNa*", 0x21, length($connectStr), $connectStr);
#	print STDERR xd $data;
#	syswrite $fh, $data;
} || die "failed to connect";

my $input; $input = AnyEvent->io (
	fh => \*STDIN,
	poll => "r",
	cb => sub {
		chomp(my $name = <STDIN>);
		my $cmd = $name;
		length($cmd) > 0 || return;
		print STDERR "cmd = $name\n";
		if ($name =~ m/^@(.*)/) {
			my $fname = $1;
			unless (-e $fname) {
				warn "failed to read $fname\n";
				return;
			}
			$cmd = read_file($fname);
		} else {
			my %msg;
			$msg{argument} = $name;
			$msg{msg} = "execScript";
			$cmd = encode_json(\%msg);
		}
		&sendMessage($cmd);
	});

$SIG{'INT'} = sub { &sendMessage('{"msg":"close"}'); $con_ready->send; };

$con_ready->recv;

sub sendMessage {
	my ($msg) = @_;
	print STDERR "writing $msg\n";
	my $data = pack("NNa*", 0x21, length($msg), $msg);
	$handle->push_write($data);
#	syswrite $fh, $data;
}
