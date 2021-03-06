/*
 * This software is distributed under BSD 3-clause license (see LICENSE file).
 *
 * Authors: Soeren Sonnenburg, Sergey Lisitsyn, Heiko Strathmann, Chiyuan Zhang, 
 *          Thoralf Klein, Shashwat Lal Das, Viktor Gal, Fernando Iglesias, 
 *          Björn Esser
 */

#ifndef _ONLINELIBLINEAR_H__
#define _ONLINELIBLINEAR_H__

#include <shogun/lib/config.h>

#include <shogun/lib/SGVector.h>
#include <shogun/lib/common.h>
#include <shogun/base/Parameter.h>
#include <shogun/machine/OnlineLinearMachine.h>

namespace shogun
{
/** @brief Class implementing a purely online version of CLibLinear,
 * using the L2R_L1LOSS_SVC_DUAL solver only. */

/** @brief This class provides an interface to the LibLinear library for large-
 * scale linear learning [1] focusing on SVM. This is the online-classification interface. For
 * batch classification, see CLibLinear, for batch regression, see
 * CLibLinearRegression.
 *
 * This class offers ::L2R_L1LOSS_SVC_DUAL only.
 * See the ::LIBLINEAR_SOLVER_TYPE enum for types of solvers for batch SVM.
 *
 * [1] http://www.csie.ntu.edu.tw/~cjlin/liblinear/
 * */
class COnlineLibLinear : public COnlineLinearMachine
{
public:

		/** problem type */
		MACHINE_PROBLEM_TYPE(PT_BINARY);

		/** Default constructor */
		COnlineLibLinear();

		/**
		 * Constructor
		 *
		 * @param C Cost constant C
		 */
		COnlineLibLinear(float64_t C);

		/**
		 * Constructor
		 *
		 * @param C Cost constant C
		 * @param traindat Training examples
		 */
		COnlineLibLinear(float64_t C, CStreamingDotFeatures* traindat);

		/**
		 * Copy Constructor
		 * @param mch another COnlineLibLinear machine
		 */
		COnlineLibLinear(COnlineLibLinear *mch);

		/** Destructor */
		virtual ~COnlineLibLinear();

		/**
		 * Set C1 and C2 constants
		 *
		 * @param c_neg C1 value
		 * @param c_pos C2 value
		 */
		virtual void set_C(float64_t c_neg, float64_t c_pos) { C1=c_neg; C2=c_pos; }

		/**
		 * Get constant C1
		 *
		 * @return C1
		 */
		virtual float64_t get_C1() { return C1; }

		/**
		 * Get constant C2
		 *
		 * @return C2
		 */
		float64_t get_C2() { return C2; }

		/**
		 * Set whether to use bias or not
		 *
		 * @param enable_bias true if bias should be used
		 */
		virtual void set_bias_enabled(bool enable_bias) { use_bias=enable_bias; }

		/**
		 * Check if bias is enabled
		 *
		 * @return If bias is enabled
		 */
		virtual bool get_bias_enabled() { return use_bias; }

		/** @return Object name */
		virtual const char* get_name() const { return "OnlineLibLinear"; }

		/** start training */
		virtual void start_train();

		/** stop training */
		virtual void stop_train();

		/** train on one example
		 * @param feature the feature object containing the current example. Note that get_next_example
		 *        is already called so relevalent methods like dot() and dense_dot() can be directly
		 *        called. WARN: this function should only process ONE example, and get_next_example()
		 *        should NEVER be called here. Use the label passed in the 2nd parameter, instead of
		 *		  get_label() from feature, because sometimes the features might not have associated
		 *		  labels or the caller might want to provide some other labels.
		 * @param label label of this example
		 */
		virtual void train_example(CStreamingDotFeatures *feature, float64_t label);

private:
		/** Set up parameters */
		void init();

		/** train on one vector
		 * @param ex the example being trained
		 * @param label label of this example
		 */
		void train_one(SGVector<float32_t> ex, float64_t label);

		/** train on one *sparse* vector
		 * @param ex the example being trained
		 * @param label label of this example
		 *
		 */
		void train_one(SGSparseVector<float32_t> ex, float64_t label);

private:
		/// use bias or not
		bool use_bias;
		/// C1 value
		float64_t C1;
		/// C2 value
		float64_t C2;

private:
		//========================================
		// "local" variables used during training

		float64_t C, d, G;
		float64_t QD;

		// alpha for example being processed
		float64_t alpha_current;

		// Cost constants
		float64_t Cp;
		float64_t Cn;

		// PG: projected gradient, for shrinking and stopping
		float64_t PG;
		float64_t PGmax_old;
		float64_t PGmin_old;
		float64_t PGmax_new;
		float64_t PGmin_new;

		// Diag is probably unnecessary
		float64_t diag[3];
		float64_t upper_bound[3];

		// Objective value = v/2
		float64_t v;
		// Number of support vectors
		int32_t nSV;
};
}
#endif // _ONLINELIBLINEAR_H__
