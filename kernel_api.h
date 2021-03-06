/*
 * kernel_api.h
 *
 *  Created on: 08 мая 2014 г.
 *      Author: pavel
 */

#ifndef KERNEL_API_H_
#define KERNEL_API_H_

namespace nnsim{

	void init_network(float h, int NumNeur, int NumConns, float SimTime);

	void init_neurs(float* a_arr, float* b1_arr, float* b2_arr, float* c_arr, float* d_arr, float* k_arr,
			float* Cm_arr, float* Erev_exc_arr, float* Erev_inh_arr, float* Ie_arr, float* Isyn_arr, float* tau_psc_exc_arr, float* tau_psc_inh_arr,
			float* Um_arr, float* Vm_arr, float* Vpeak_arr, float* Vr_arr, float* Vt_arr, float* p1_arr, float* p2_arr);

	void init_synapses(float* tau_rec_arr, float* tau_fac_arr, float* U_arr,
			float* x_arr, float* y_arr, float* u_arr, float* weights_arr, float* delays_arr,
			unsigned int* pre_conns_arr, unsigned int* post_conns_arr, unsigned int* receptor_type_arr);

	void init_spikes(unsigned int* spike_times, unsigned int* neur_num_spikes,
			unsigned int* syn_num_spikes, unsigned int spk_times_len);

	int simulate(int useGPU);

	void init_recorder(unsigned int neur_num, unsigned int* neurs, unsigned int con_num, unsigned int* conns);

	void get_neur_results(float* &Vm_res, float* &Um_res, float* &Isyn_res, float* &y_exc_res, float* &y_inh_res, unsigned int &N);

	void get_conn_results(float* &x_res, float* &u_res, unsigned int &N);

	void get_mean_neur_results(float* &Vm_res, float* &Um_res, float* &Isyn_res, float* &y_exc_res, float* &y_inh_res, unsigned int &N);

	void get_mean_conn_results(float* &x_res, float* &u_res, unsigned int &N);

	void get_spike_times(unsigned int* &spike_times, unsigned int* &num_spikes_on_neur);

	void init_poisson(unsigned int* seeds, float* rates, float* weights, float psn_tau);

	void init_mean_recorder(unsigned int num_pop_neur, unsigned int num_pop_conn);

	void add_neur_mean_record(unsigned int pop_size, unsigned int* pop_neurs);

	void add_conn_mean_record(unsigned int pop_size, unsigned int* pop_conns);

}

#endif /* KERNEL_API_H_ */
