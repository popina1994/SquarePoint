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

/*

○ The question is whether we can measure exactly when 1ms pass or it can be approximate. Approximate makes sense.
○ Circular buffer to keep the sum of squares and sum alongside the numbers
	§ Create a circular buffer that will have one end Idx and beginIdx, and multiple  startIdx where the sliding window for each of the sliding windows begins
		□ Downside, latency due to having to store all these numbers. For bigger sliding windows, update them less frequently base on smaller latencies since they will be approximately correct due to the law of large numbers? For example 1m sounds fine to me to other latencies
		□ Have circular buffer search in logarithmic time?
		□ Try to have timer that would store sum of squares and and sum every 1ms alongside the number of elements. Then update all other sums based on this computation.
○ Performance
	§ First parallelize searches in several threads and compute mean/variance in several threads
	§ In one thread just push back and resize by throwing out the old data
○ Accumulating error for floating point????
At start of each day start computing mean from scratch and once sufficient time interval has passed switch the existing mean to the newly computed mean*/