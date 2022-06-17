# kbdconf

My ad-hoc keyboard configurtaion using interception-tools.

usage:

```bash
DEVNODE=/dev/input/event[n] intercept -g $DEVNODE | kbdconf | uinput -d $DEVNODE
```
