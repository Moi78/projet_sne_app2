# Demo SNE APP2

### Arborescence

- `src/` : Source du firmware
- `soft_pc/` : Source du logiciel côté PC qui drive le port série et envoie la capture de l'écran
- `lib/` : Drivers
  - `Adafruit_GFX` : Driver d'adafruit porté pour Mbed et légèrement adapté pour fonctionner avec notre ecran
  - `nils_lib` : Drivers écrits par Nils
  - `rgb_matrix` : Driver de la matrice RGB