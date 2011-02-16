#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>


class Utility
{
public:
    // string utilities
	static void ResolveFileName(const std::string& filename, std::string& file_path, 
		std::string& file_title, std::string& file_ext);
	static void MakeLower(std::string& str);
	static void MakeUpper(std::string& str);

    // Free various vectors
    template <class T>
    void FreeVector(T& arr)
    {
        T tmp;
        arr.clear();
        arr.swap(tmp);
    }

    // Flag utilities
    template <class T>
        void SetFlag(T& flag_ele, T flag) { flag_ele |= flag; }

    template <class T>
        void ClearFlag(T& flag_ele, T flag) { flag_ele &= ~flag; }

    template <class T>
        bool IsSetFlag(T& flag_ele, T flag) { return ((flag_ele&flag) == flag); }

    template <class T>
        void ToggleFlag(T& flag_ele, T flag)
    {
        if(IsSetFlag(flag_ele, flag))
            ClearFlag(flag_ele, flag);
        else
            SetFlag(flag_ele, flag);
    }
    
    // Mean value and variance calculation
    template <class T>  // Make sure class T is a vector of basic data type (int, float, double, etc... )
	void CalMeanAndStdVar(std::vector<T>& arr, T& mean, T& std_var)
    {
        int n = arr.size();
        if(n == 0)
            return;

        double Mean = 0.0, Var = 0.0;
        for(int i = 0; i < n; ++ i)
            Mean += arr[i];
        Mean /= (double)n;

        for(int i = 0; i < n; ++ i)
        {
            double tmp = arr[i]-Mean;
            Var += tmp*tmp;
        }
        Var /= (double)n;

        mean = (T)Mean;
        std_var  = (T)sqrt(Var);
    }

    // 
    template <class T>  // Make sure class T is a vector of basic data type (int, float, double, etc... )
	bool IsInVector(std::vector<T>& arr, T& value)
    {
		size_t n = arr.size();
        if(n == 0)
            return false;

        for(size_t i = 0; i < n; ++ i)
		{
			if (arr[i] == value)
			{
				return true;
			}
		}

		return false;
	}

	template <class T>  // Make sure class T is a vector of basic data type (int, float, double, etc... )
	int IndexInVector(std::vector<T>& arr, T value)
	{
		int n = (int) arr.size();
		if(n == 0)
			return -1;

		for(int i = 0; i < n; ++ i)
		{
			if (arr[i] == value)
			{
				return i;
			}
		}

		return -1;
	}
	template <class T>  // Make sure class T is a vector of basic data type (int, float, double, etc... )
	void ReverseVector(std::vector<T>& arr)
	{
      std::vector<T> tmp_arr(arr);

		int n = (int) arr.size();
		if(n == 0)
			return;

		for(int i = 0; i < n; ++ i)
		{
			arr[n- 1 - i] = tmp_arr[i]; 
		}
	}

    // Data set filtering
    template <class T>  // Make sure class T is a vector of basic data type (int, float, double, etc... )
	int DataClamp(std::vector<T>& arr, T mean, T std_var, double coef)
    {
        int n = arr.size();
        int nClamp = 0;
        for(int i = 0; i < n; ++ i)
        {
            T& data = arr[i];
            double tmp = (data-mean)/std_var;
            if(tmp > coef)
            {
                data = coef*std_var + mean;
                ++ nClamp;
            }
            else if(tmp < -coef)
            {
                data = -coef*std_var + mean;
                ++ nClamp;
            }
        }
        
        return nClamp;
    }


	template<class T >
        T NoramlizeVector(std::vector<T>& vec)
	{
        T sum = 0;
		for (size_t i = 0; i < vec.size(); i++)
		{
			sum += vec[i] * vec[i];
		}

		sum = sqrt(sum);

		for (size_t i = 0; i < vec.size(); i++)
		{
			vec[i] /= sum;
		}
		return sum;
	}

    template<class T> // Make sure that T is a index array
    void Unique(T& arr)
    {
        sort(arr.begin(), arr.end());
        arr.erase(unique(arr.begin(), arr.end()), arr.end());
    }
};
