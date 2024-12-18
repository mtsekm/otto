# otto

Otto is a lightweight and extensible command automation tool designed for simulating key events, executing predefined command sequences, and recording IR key events for playback.


## **Basic Usage**

### **Command Syntax**

Commands are defined in a plain text file with one command per line. Supported commands include:

| Command         | Description                                                                 | Example                             |
|-----------------|-----------------------------------------------------------------------------|-------------------------------------|
| `key_press`     | Simulate a key press event. Optionally specify repeat count.                | `key_press power 3`                 |
| `loop_start`    | Begin a loop block with a specified repeat count.                           | `loop_start 2`                      |
| `loop_end`      | End the current loop block.                                                 | `loop_end`                          |
| `wait`          | Introduce a delay. Supports `ms`, `s`, or `m` units.                        | `wait 2s`                           |
| `var`           | Define a variable for later use.                                            | `var x 5`                           |
| `launch_app`    | Launches an application by its app ID.                                      | `launch_app YouTube`                |
| `close_app`     | Closes an application by its app ID.                                        | `close_app YouTube`                 |

### **Record Mode**

Otto can also operate in record mode, capturing IR key events from a remote control and saving them as `key_press` commands in a specified file.

#### **Starting Record Mode**

Run Otto in record mode with the `--record` option:

```bash
./otto --record=recorded_commands.txt
```

#### **Example of a Recorded File**
```
key_press power
key_press volup
key_press voldown
key_press mute
```

These commands can be replayed later by providing the recorded file as input to Otto.

### **Running Otto**

1. **Prepare a Commands File**  
   Create a text file (e.g., `commands.txt`) with the desired sequence of commands:
   ```
   var app YouTube
   launch_app $app
   loop_start 2
   key_press right 3
   key_press down
   key_press left 3
   key_press up
   loop_end
   close_app $app
   ```

2. **Run Otto with the Commands File**  
   Execute Otto by providing the commands file as input:
   ```
   ./otto commands.txt
   ```
