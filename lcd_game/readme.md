Built using [platformio](http://platformio.org/). You can built it with any other tool provided you compile the LCD library, in lib/liblcd.

Here's custom board descriptor:
ilmatto.json
```json
{
  "build": {
    "f_cpu": "12000000L",
    "mcu": "atmega644p",
    "variant": "standard"
  },
  "frameworks": [],
  "name": "Il Matto",
  "upload": {
    "maximum_ram_size": 4096,
    "maximum_size": 64512,
    "protocol": "usbasp",
    "extra_flags": "-Pusb",
    "speed": 115200
  },
  "url": "https://www.ecs.soton.ac.uk/",
  "vendor": "University of Southampton"
}
```

You can either put that in ~/.platformio/boards or in a boards directory within your project.