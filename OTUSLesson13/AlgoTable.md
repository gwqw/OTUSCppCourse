**Bulk** has: state, buffer, max_buffer_size == capacity, counter
**States**: General, Custom
**Commands**: Base, Open, Close, Terminate


State   |Command    |Result state |Action
--------|-----------|-------------|------------
General |Base       |General      |add to buffer; if buffer.size >= capacity: flush buffer
General |Open       |Custom       |flush buffer 
General |Close      |General      |error: cerr this error
General |Terminate  |General      |flush buffer
Custom  |Base       |Custom       |add to buffer
Custom  |Open       |Custom       |inc counter
Custom  |Close      |Cust or Gen  |dec counter, if counter == 0: flush and switch to General state
Custom  |Terminate  |General      |empty buffer
