use Digest::SHA1 qw(sha1_base64);

$magic_salt='258EAFA5-E914-47DA-95CA-C5AB0DC85B11';

$client='dGhlIHNhbXBsZSBub25jZQ==';
$expect='s3pPLMBiTxaQ9kYGzzhZRbK+xOo=';

$client='mr7YSUQyjXimPSpeV7hIfg==';
$expect='oBzVZCLQ8fVr0WkpdrAx9uyQjYM=';

$ret=sha1_base64($client.$magic_salt);
$ret.='=';

print "we got: $ret, expect: $expect, is ".($ret eq $expect?"ok":"fail")."\n";
