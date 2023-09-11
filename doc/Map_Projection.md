# $Map\ Projection$

[TOC]

Map Projection is the process of converting a 3D sphere into a 2D plane. Representing sphere on a flat plane inevitably introduces some distortions in properties such as distance, area, shape, or direction. Different map projections are designed to preserve certain properties while sacrificing others, depending on the specific purpose and area of interest.

# Classification by method
## Cylindrical Projection

### Mercator Projection

$$
X = \lambda  \\
Y = \ln(\tan(\frac{\pi}{4} + \frac{\phi}{2}))
$$

<img src="./assets/1024px-Mercator_with_Tissot's_Indicatrices_of_Distortion.svg.png" alt="undefined" style="zoom: 20%;" />

<img src="./assets/1024px-Cylindrical_Projection_basics2.svg.png" alt="undefined" style="zoom:33%;" />

### Lambert Cylindrical Projection

$$
X = \lambda  \\
Y = \sin(\phi)
$$

<img src="./assets/Tissot_indicatrix_world_map_Lambert_cyl_equal-area_proj.svg" alt="Tissot_indicatrix_world_map_Lambert_cyl_equal-area_proj" style="zoom:12%;" />

## Pseudocylindrical Projection

### Mollweide projection

<img src="./assets/1024px-Mollweide_with_Tissot's_Indicatrices_of_Distortion.svg.png" alt="undefined" style="zoom: 30%;" />

The Mollweide projection is an equal-area, pseudocylindrical projection in which the equator is represented as a straight horizontal line perpendicular to a central meridian that is one-half the equator's length. The other parallels compress near the poles, while the other meridians are equally spaced at the equator. The meridians at 90 degrees east and west form a perfect circle, and the whole earth is depicted in a proportional 2:1 ellipse. The proportion of the area of the ellipse between any given parallel and the equator is the same as the proportion of the area on the globe between that parallel and the equator, but at the expense of shape distortion, which is significant at the perimeter of the ellipse, although not as severe as in the sinusoidal projection.

### Sinusoidal projection

<img src="./assets/1024px-Sinusoidal_with_Tissot's_Indicatrices_of_Distortion.svg.png" alt="undefined" style="zoom:30%;" />

<img src="./assets/Usgs_map_sinousidal_equal_area.png" alt="undefined" style="zoom: 50%;" />

### Boggs eumorphic projection

Boggs generally repeated regions in two different lobes of the interrupted map in order to show Greenland or eastern Russia undivided.

<img src="./assets/Boggs_eumorphic_projection_SW.jpeg" alt="img" style="zoom:15%;" />

<img src="./assets/1024px-Boggs_eumorphic_projection_Tissot.svg.png" alt="undefined" style="zoom: 30%;" />




## Conic Projection.

### Lambert Conformal Conic Projection

$$
X = \rho \sin(\lambda - \lambda_0)  \\
Y = \rho_0 - \rho \cos(\lambda - \lambda_0)
$$

<img src="./assets/1024px-Conformal_Conic_with_Tissot's_Indicatrices_of_Distortion.svg.png" alt="undefined" style="zoom:25%;" />

## Azimuthal Equidistant Projection

$$
X = R \cos(\phi) \sin(\lambda - \lambda_0)  \\
Y = R \left(\cos(\phi_0) \sin(\phi) - \sin(\phi_0) \cos(\phi) \cos(\lambda - \lambda_0)\right)
$$

<img src="./assets/Azimuthal_equidistant_projection_with_Tissot's_indicatrix.png" alt="Tissot’s indicatrix applied to the azimuthal equidistant projection" style="zoom:30%;" />

# Classification by characteristic
## Equal-distance Projection

## Equal-area Projection

- Azimuthal
    - Lambert azimuthal equal-area
    - Wiechel

- Conic
    - Albers
    - Lambert equal-area conic projection

- Pseudoconical
    - Bonne
    - Bottomley
    - Werner

- Cylindrical
    - Lambert cylindrical equal-area (0°)
    - Behrmann (30°)
    - Hobo–Dyer (37°30′)
    - Gall–Peters (45°)

- Pseudocylindrical
    - Boggs eumorphic
    - Collignon
    - Eckert II, IV and VI
    - Equal Earth
    - Goode's homolosine
    - Mollweide
    - Sinusoidal
    - Tobler hyperelliptical
- Eckert-Greifendorff
- McBryde-Thomas Flat-Polar Quartic Projection
- Hammer
- Strebe 1995
- Snyder equal-area projection
