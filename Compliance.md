The plan is for Drip Scheme to be mostly compliant, and eventually supporting multiple srfis as well as chicken and gambit's extensions.

However most backends should have a strict, a lenient mode, and a quick mode. The lenient mode (the default) should use the backends builtin string/conversion functions in a way
that is relatively compliant. The quick mode should use standard backend functions in the most convenient and performant ways possible. Strict mode should attempt to follow the 
standard precisely.
