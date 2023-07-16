/*
○ Algorithms for computing
	§ Standard 1 - pass:
		□ compute sum and sum of squares, then use similar formula as with integers.
		□ Error: Nk^2 * u
		□ For low variance the cancellation can appear, thus it can produce negative values.
	§ Standard 2 - pass:
		□ Compute mean, then use mean to compute variance
		□ Types
			® Classical
				◊ (should be avoided)
				◊ Error: Nu + N^2 * k^2 * u^2
			® Pairwise
				◊ ?
				◊ Error: log N * u + (k * u * log N)^2
			® Corrected
				◊ ?
				◊ Error: NU + N^3 * k^2 * u^2
			® Corrected pairwise
	§ Updating
		□ Recurrent formula (doesn't use too much memory)
		□ No negative values
		□ Types
			® Incremental?
				◊ Error: log N * k * u
			® Pairwise?
				◊ Error:  N * k * u

Updating algorithm should be used to avoid non-negative values
*/