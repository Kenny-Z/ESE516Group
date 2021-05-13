## Links
Video Link:https://youtu.be/Bk4TMzykF_s
More Detail in: https://devpost.com/software/voodoomagic
Dashboard：https://voodoo-magic-ese516.mybluemix.net/ui/#!/0?socketid=0TbA7WDdFbfK_o4HAAAL****
## Inspiration
Simon Says game is a golden memory of many for their childhood. As we grew up, it's harder to get ground with our friends and play an easy and entertaining party game. That's why we created this gaming board with IoT technology to provide an online gaming experience, yet keeps all the laugh and joy from a golden memory.
## What it does
Our system has an online platform that connects two players and provides a joyful multi-player gaming experience with a lighting keyboard and an Oled screen. The keyboard will light up in sequence to show the last move from the other player, while Oled provides status information. During the game, each player should take turns to follow the move from the keyboard, and add one more key each time. 
 [Node Red Link:](https://voodoo-magic-ese516.mybluemix.net/red/#flow/1e8bd1ca.15926e)
There is a link to our github page and a dash board link at the end.
## How we built it
On our PCB, we have a microcontroller and a WiFi chip that connects us to an MQTT broker and a cloud-based platform built with Node Red. A keyboard with LED and an Oled screen are used as a local user interface to show the current move, gaming status, and collect the moves from the player.
Players' moves are sent and processed by the online platform. Our main firmware is built with FreeRTOS for task control. A bootloader and an SD-card file system are designed for HTTP firmware updates.

## Challenges we ran into
Most of the sensors and interfaces are communicating with the I2C protocol in our system. Even with mutexes and semaphores provided in FreeRTOS, some concurrency and collision problems still exist, which requires careful thought into priorities and preemptions. And memory management was also a big challenge when adding FreeRTOS tasks, stack overflow would happen. To address this issue, we needed to optimize the tasks to the essential ones to save more space in the stack. We built our own custom PCB for the hardware part, where we come across many challenges in schematic designs and PCB layout.
## Accomplishments that we're proud of
The main firmware is stable and we achieve concurrency with all the tasks of controlling, WiFi communication, keyboard/Oled user interface, command-line interface, and light sensing. We used, modified, and even created some device drivers from scratch. The PCB layout looks neat and organized with square edges.
## What we learned
Power architecture for an embedded device. PCB design and layout. Wireless communication protocols. Embedded firmware with real-time operating systems. IoT structure and cloud services. Documentation practices. Code reusability and portability (hardware abstraction layer).
## What's next for VoodooMagic
Power architecture should be improved for our board, and we hope to add more IoT features to our design.
For now, let's play Simon Says Game for a while, and look for more interesting projects!
