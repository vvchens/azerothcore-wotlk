#!/usr/bin/env bash
set -e

# Ensure config directory exists and has proper permissions
if [ ! -f /azerothcore/env/dist/etc/worldserver.conf ]; then
    cp /azerothcore/env/dist/etc/worldserver.conf.dist /azerothcore/env/dist/etc/worldserver.conf 2>/dev/null || true
fi

if [ ! -f /azerothcore/env/dist/etc/authserver.conf ]; then
    cp /azerothcore/env/dist/etc/authserver.conf.dist /azerothcore/env/dist/etc/authserver.conf 2>/dev/null || true
fi

# Execute the command
exec "$@"