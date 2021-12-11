Design choices:

Canvas2D stores member variables for the current brush, brush type, color and radius. These change at the settingsChanged call. All brushes store masks as std::vector<float> and SmudgeBrush stores the paint it has picked dup as std::vector<RGBA>.

No known bugs.
