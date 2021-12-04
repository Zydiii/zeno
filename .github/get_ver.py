import datetime
import platform
import sys

d = datetime.datetime.now()
date = '{}.{}.{}'.format(d.year, d.month, d.day)

osys = {
        'linux': 'linux',
        'win32': 'windows',
        'darwin': 'macos',
}.get(sys.platform, sys.platform)

mach = {
        'AMD64': 'x86_64',
}.get(platform.machine().lower(), platform.machine().lower())

if osys == 'windows':
    libc = platform.win32_ver()[1]
else:
    libc = ''.join(platform.libc_ver())

arch = '{}-{}-{}'.format(mach, osys, libc)

print('::set-output name=date::{}'.format(date))
print('::set-output name=arch::{}'.format(arch))