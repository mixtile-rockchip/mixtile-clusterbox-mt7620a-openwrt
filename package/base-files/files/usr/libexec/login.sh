#!/bin/sh

[ "$(uci -q get system.@system[0].ttylogin)" = 1 ] || exec /bin/bash --login

exec /bin/login
