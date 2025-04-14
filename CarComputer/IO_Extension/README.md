# Brightness Sensitity Measure

To assess the performance of our solar cells, we need to measure the brightness of the surrounding daylight. This helps us determine if the solar cells are underperforming. To achieve this, we will connect a photodiode to the analog output (AO) on Arduino or pin 49 on the P2. Additionally, we will add a capacitor to minimize voltage ripples and spikes, and include a resistor to keep the current low.

# Schematic

![Alt Text](https://drive.google.com/uc?export=view&id=19vkGaPTyZuv4kTRvDNJbMRsRLqKQnVPM)

# Brightenss

![Alt Text](https://drive.google.com/uc?export=view&id=1-YKOJx6ln_dEuKU-ub7NoreBVVY-lmM)

Through this program, we expect to receive a percentage indicating the light level detected by the photocell. A reading of 100% represents the maximum light exposure, while 0%-10% indicates extremely dark conditions. A 50% reading corresponds to typical indoor lighting.
