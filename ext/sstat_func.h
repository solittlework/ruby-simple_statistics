#ifndef _SSTAT_FUNC_H_
#define _SSTAT_FUNC_H_
struct point {
	double x;
	double y;
}

double precentile(double* array, int size, double target_percentile)
{
	double percentage_each = 1.0 / size;
	int i;
	int count = 0;
	for(i = 1; i <= size; i++)
	{
		if ( i * percentage_each < target_percentile )
		{
			count++;
		} else
		{
			return array[count];
		}
	}

	return array[size -1];
}

int index_less_equal(double*array, int size, double target)
{
	int i;
	for(i = 0; i < size; i++)
	{
		if(array[i] > target)
			return i;
	}
	return i;
}

int index_less_equal(double*array, int size, double target)
{
	int i;
	for(i = 0; i < size; i++)
	{
		if(array[i] > target)
			return i;
	}

	return i;
}

point* kaplan_meier(double* event_time, bool* censored, int size)
{
	quick_sort_simultaneous(event_time, censored, size);
	int i;	
	// extract uncensored data points
	vector<sample_type> sample_uncens;
	int uncensored_num = 0;
	
	for(i = 0; i < size; i++)
	{
		if(censored[i] == false)
			uncensored_num += 1;
	}

	double uncensored_time(double *)malloc(sizeof(double)*size);

	// Kaplan-Meier curve changes only at times when there is an uncensored event
	vector<D_T> event_time_uncens;

	// start graph at time=0
	event_time_uncens.push_back(0);

	for(auto& s : sample_uncens)
	{
		event_time_uncens.push_back(s.second);
	}

	//remove the duplicates in the event_time_uncens
	event_time_uncens.erase (
		unique (
			event_time_uncens.begin(),
			event_time_uncens.end()
		),
		event_time_uncens.end()
	);

	
	D_T bound = event_time_uncens.front();

	/* 
	   event_time_uncens = {0, t1, t2, t3, ..., tn} (t_i < t_{i+1})
	   we now need to partition samples into groups g_i where each sample in 
	   g_i has event time e_i with  t_{i-1} < e_i <= t_i
	*/

	/* first -> number of uncensored in group i
		second -> nubmer of censored in group i
	*/
	pair<size_t, size_t> group_i;


	size_t group_number = 0; //remember first in event_time_uncens is 0, which we start at probability 1
	for(auto & s : samples)
	{
		if(s.second <= bound) // event occurs before t_i
		{	//censored
			if(s.first)
				group_i.second++;
			else
				group_i.first++;
		} else { // event occurs after t_i, so finished with g_i

			KM_group.push_back(group_i); // add g_i to our queue

			// initialize g_{i+1}
			group_i.first = 0;  
			group_i.second = 0;

			if (event_time_uncens.size()==group_number+1)
				break;
			bound = event_time_uncens.at(group_number + 1);
			group_number++;

			if(s.first)
				group_i.second++;
			else
				group_i.first++;
		}
	}

	D_T n_i = static_cast<D_T>(sample_len);  // number of samples surviving up to g_i
	D_T d_i = 0;                             // number of uncensored events in g_i
	D_T c_i = 0;                             // number of censored events in g_i
	D_T KM_i = 1;                            // = probability of event occurring after t_i  (p(0)=1)

	Array KM_p;

	for(size_t i = 0; i < KM_group.size(); ++i)
  	{
		if (i>0) { // remove samples who's events occurred in previous interval
      		n_i = n_i - KM_group[i-1].first - KM_group[i-1].second;
    	}

    	d_i = KM_group[i].first;
    	c_i = KM_group[i].second;

    	// our convention: if censored event occurs at same time as uncensored, we assume censored event happened "nanoseconds" before
    	// eg. if 100 people initially, and 1 death and 1 censoring at time 1, we assume prob of living past time 1 is 98/99 (rather than 99/100 if opposite convention)
      	KM_i = static_cast<D_T>(n_i - d_i - c_i) / (n_i - c_i) * KM_i;
    	KM_p.push_back(KM_i);
	}
	
	// now combine probabilities with corresponding times into one data structure
	Curve KM;

	for(size_t i = 0; i < event_time_uncens.size(); ++i)
  	{
		KM.x_axis.push_back(event_time_uncens[i]);
		KM.y_axis.push_back(KM_p[i]);
	}

	if(KM.y_axis.back()!= 0)
	{
		KM.y_axis.back() = 0;
	}

	/**
	 if KM does not end with zero, we extend it with line of best fit through the final 3 points.
	*/
	if(KM.y_axis.back() != 0)
	{
		if(DEBUG)
		{
			write_log(string("A"), string("KM_extend_log"));
		}

		size_t KM_size = KM.x_axis.size();
		Array last_time_points;
		Array last_KM_prob_points;
		size_t num_for_fitting = 5 > KM_size ? KM_size: 5;
		if(DEBUG)
		{
			write_log(string("Before linear regression"), string("KM_extend_log"));
		}

		string last_KM_log = "";

		// collect final N points of current KM curve for fitting a line
		for(int i = num_for_fitting; i > 0 ; i--)
		{

			last_KM_log += ( string("(") + to_string(KM.x_axis[KM_size - i]) + string(", ") + to_string(KM.y_axis[KM_size - i]) + string("), "));

			last_time_points.push_back(KM.x_axis[KM_size - i]);
			last_KM_prob_points.push_back(KM.y_axis[KM_size - i]);
		}

		if(DEBUG)
		{
			write_log(last_KM_log, string("KM_extend_log"));
		}
		// determine slope and intercept of best line
		auto line = linear_regression(last_time_points, last_KM_prob_points);

		if(DEBUG)
		{
			write_log(string("After linear regression"), string("KM_extend_log"));
			write_log(to_string(line.slope), string("KM_extend_log"));
			write_log(to_string(line.intercept), string("KM_extend_log"));
		}

		assert(line.slope < 0);

		// determine x-intercept so we know when to stop adding points to curve
		double x_intercept = -line.intercept/line.slope;
		double time_spacing = (last_time_points.back() - last_time_points.front())/(last_time_points.size()-1);
		assert(time_spacing > 0);

		// add equally spaced points to KM curve along line of best fit
		double t = last_time_points.back()+time_spacing;

		while (t < x_intercept)
		{
			double prob = t*line.slope + line.intercept;
			if (prob > KM.y_axis.back()) // line of best fit doesn't necessarily go through final point, so make sure we are decreasing
				prob = KM.y_axis.back();
			KM.x_axis.push_back(t);
			KM.y_axis.push_back(prob);
			t += time_spacing;
		}
		// end KM curve at 0
		KM.x_axis.push_back(x_intercept);
		KM.y_axis.push_back(0);
	} 

	return KM;
}



#endif
