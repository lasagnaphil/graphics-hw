#!/usr/bin/env bash

rm -rf glad

case "$1" in
    release-3.3)
        glad --profile=core --api="gl=3.3" --generator=c --out-path=glad
        ;;
    release-4.3)
        glad --profile=core --api="gl=4.3" --generator=c --out-path=glad
        ;;
    debug-3.3)
        glad --profile=core --api="gl=3.3" --generator=c-debug --extensions=GL_ARB_debug_output --out-path=glad
        ;;
    debug-4.3)
        glad --profile=core --api="gl=4.3" --generator=c-debug --extensions=GL_ARB_debug_output --out-path=glad
        ;;
esac
