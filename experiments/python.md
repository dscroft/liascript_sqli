<!--
author:   André Dietrich
email:    LiaScript@web.de
version:  0.3.2
language: en
narrator: US English Male

logo:     logo.jpg

comment:  Use the real Python in your LiaScript courses, by loading this
          template. For more information and to see, which Python-modules are
          accessible visit the [pyodide-website](https://alpha.iodide.io).

import: https://raw.githubusercontent.com/LiaTemplates/Pyodide/master/README.md
-->


# Learn Python

## How?

#### Formatted code

Putting just the code in the file done as:

````markdown
```python
print("Hello World!")
```
````

```python
print("Hello World!")
```

#### Executable code

Import the following in the header of your markdown document:

```markdown
<--
import: https://raw.githubusercontent.com/LiaTemplates/Pyodide/master/README.md
-->
```

````markdown
```python
import time

print(f"Hello World! The time is {time.time()}")
```
@Pyodide.eval
````

```python
import time

print(f"Hello World! The time is {time.time()}")
```
@Pyodide.eval


#### Auto-Execution

````markdown
```python   @Pyodide.exec
import sys

for i in range(5):
	print("Hello", 'World #', i)

sys.version
```
````

```python   @Pyodide.exec
import sys

for i in range(5):
	print("Hello", 'World #', i)

sys.version
```

## Control Flow

### Conditional statements: if, elif, else

The Python syntax for conditional execution of code uses the keywords `if`, `elif` (else if), `else`:

```python
if statement1:
    print("statement1 is True")
    
elif statement2:
    print("statement2 is True")
    
else:
    print("statement1 and statement2 are False")
```



For the first time, here we encountered a peculiar and unusual aspect of the Python programming language: Program blocks are defined by their indentation level. 

In Python, the extent of a code block is defined by the indentation level (usually a tab or say four white spaces). This means that we have to be careful to indent our code correctly, or else we will get syntax errors. 

#### Examples:
 
```python
statement1 = statement2 = True

if statement1:
    if statement2:
        print("both statement1 and statement2 are True")
```
@Pyodide.eval

**Bad indentation!**

```python
if statement1:
    if statement2:
    print("both statement1 and statement2 are True")  # this line is not properly indented"
```
@Pyodide.eval


```python
statement1 = False 

if statement1:
    print("printed if statement1 is True")
    
    print("still inside the if block")
```
@Pyodide.eval

## Plots

                                   --{{1}}--
If you want to use matplotlib, you will have to pass your figure to the `plot`
function, as it is done in the last line below. This function converts your
image into a base64 representation and passes this string to the DOM. It is currently only possible to plot one figure per snippet.

```python
import numpy as np
import matplotlib.pyplot as plt

t = np.arange(0.0, 2.0, 0.01)
s = np.sin(2 * np.pi * t)

fig, ax = plt.subplots()
ax.plot(t, s)

ax.grid(True, linestyle='-.')
ax.tick_params(labelcolor='r', labelsize='medium', width=3)

plt.show()
```
@Pyodide.eval


```python
import pandas as pd
d = {'col1': [1, 5, 7], 'col2': [3, .4, -2], 'col3':["yes", "no","blue"]};
df = pd.DataFrame(data=d);
df
print(df)
```
@Pyodide.eval


## `@Pyodide.exec`

This macro works similar to the previous one, but the code is only passed as a parameter.
The user will only see the result and will not have the chance to directly modify the the Python code.

```python   @Pyodide.exec
import sys

for i in range(5):
	print("Hello", 'World #', i)

sys.version
```


```python   @Pyodide.exec
import numpy as np
import matplotlib.pyplot as plt

t = np.arange(0.0, 2.0, 0.01)
s = np.sin(2 * np.pi * t)

fig, ax = plt.subplots()
ax.plot(t, s)

ax.grid(True, linestyle='-.')
ax.tick_params(labelcolor='r', labelsize='medium', width=3)

plt.show()
```

## Loading Libraries

                                   --{{0}}--

Only the Python standard library and `six` are available at the beginning, other
libraries are globally loaded, if defined within the script. If you know, that certain modules are required, you can speed up their loading by defining them
manually in your `onload` macro, as it is shown below.


``` markdown
<!--
author:  ...
email:   ...

import:  https://github.com/LiaTemplates/Pyodide

@onload: window.py_packages = ["matplotlib", "numpy"]
-->
...
```

> __Note:__ loading large packages such as `scipy` may take some time, since
>           they might require to download many MB of precompiled packages.

## Implementation

                                   --{{0}}--
This macro implementation only adds a simple script-tag that pushes the code of
your snippet directly to Pyodide. The `@onload` macro is required to instantiate
Pyodide and load the required libraries, which might require some time, since
the loaded packages might be quite large.


```` js
script:   https://cdn.jsdelivr.net/pyodide/v0.24.0/full/pyodide.js

@Pyodide.exec: @Pyodide.exec_(@uid,```@0```)

@Pyodide.exec_
<script>
async function run(code, force=false) {
    if (!window.pyodide_running || force) {
        window.pyodide_running = true
    
        const plot = document.getElementById('target_@0')
        plot.innerHTML = ""
        document.pyodideMplTarget = plot

        if (!window.pyodide) {
            try {
                window.pyodide = await loadPyodide({fullStdLib: false})
                window.pyodide_modules = []
                window.pyodide_running = true
            } catch(e) {
                send.lia(e.message, false)
                send.lia("LIA: stop")
            }
        }

        try {
            window.pyodide.setStdout((text) => console.log(text))
            window.pyodide.setStderr((text) => console.err(text))

            window.pyodide.setStdin({stdin: () => {
            return prompt("stdin")
            }})
        
            const rslt = await window.pyodide.runPython(code)
            
            if (rslt !== undefined) {
                send.lia(rslt)
            } else {
                send.lia("")
            }
        } catch(e) {
            let module = e.message.match(/ModuleNotFoundError: The module '([^']+)/i)

            window.console.warn("Pyodide", e.message)
        
            if (!module) {
                send.lia(e.message, false)
            
            } else {
                if (module.length > 1) {
                    module = module[1]

                    if (window.pyodide_modules.includes(module)) {
                        console.warn(e.message)
                        send.lia(e.message, false)
                    } else {
                        send.lia("downloading module => " + module)
                        window.pyodide_modules.push(module)
                        await window.pyodide.loadPackage(module)
                        await run(code, true)
                    }
                }
            }
        }
        send.lia("LIA: stop")
        window.pyodide_running = false
    } else {
        setTimeout(() => { run(code) }, 1000)
    }
}

setTimeout(() => { run(`@1`) }, 500)

"calculating, please wait ..."

</script>

<div id="target_@0"></div>
@end


@Pyodide.eval: @Pyodide.eval_(@uid)

@Pyodide.eval_
<script>
async function run(code) {

    const plot = document.getElementById('target_@0')
    plot.innerHTML = ""
    document.pyodideMplTarget = plot

    if (!window.pyodide) {
        try {
            window.pyodide = await loadPyodide({fullStdLib: false})
            window.pyodide_modules = []
            window.pyodide_running = true
        } catch(e) {
            console.error(e.message)
            send.lia("LIA: stop")
        }
    }

    try {
        window.pyodide.setStdout({ write: (buffer) => {
            const decoder = new TextDecoder()
            const string = decoder.decode(buffer)
            console.stream(string)
            return buffer.length
        }})

        window.pyodide.setStderr({ write: (buffer) => {
            const decoder = new TextDecoder()
            const string = decoder.decode(buffer)
            console.err(string)
            return buffer.length
        }})

        window.pyodide.setStdin({stdin: () => {
          return prompt("stdin")
        }}) 
       
        const rslt = await window.pyodide.runPython(code)

        if (typeof rslt === 'string') {
            send.lia(rslt)
        }
    } catch(e) {
        let module = e.message.match(/ModuleNotFoundError: The module '([^']+)/i)

        window.console.warn("Pyodide", e.message)
    
        if (!module) {
            const err = e.message.match(/File "<exec>", line (\d+).*\n((.*\n){1,3})/i)

            if (err!== null && err.length >= 3) {
                send.lia( e.message,
                  [[{ row : parseInt(err[1]) - 1,
                      column : 1,
                      text : err[2],
                      type : "error"
                  }]],
                  false)
            } else {
                console.error(e.message)
            }
        } else {
            if (module.length > 1) {
                module = module[1]

                if (window.pyodide_modules.includes(module)) {
                    console.error(e.message)
                } else {
                    console.debug("downloading module =>", module)
                    window.pyodide_modules.push(module)
                    await window.pyodide.loadPackage(module)
                    await run(code)
                }
            }
        }
    }
    send.lia("LIA: stop")
    window.pyodide_running = false
}

if (window.pyodide_running) {
  setTimeout(() => {
    console.warn("Another process is running, wait until finished")
  }, 500)
  "LIA: stop"
} else {
  window.pyodide_running = true

  setTimeout(() => {
    run(`@input`)
  }, 500)

  "LIA: wait"
}
</script>

<div id="target_@0"></div>
@end
````

                                   --{{1}}--
If you want to minimize loading effort in your LiaScript project, you can also
copy this code and paste it into your main comment header, see the code in the
raw file of this document.


                                     {{1}}
https://raw.githubusercontent.com/LiaTemplates/pyodide/master/README.md
