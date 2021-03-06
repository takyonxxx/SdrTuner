#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/gr_complex.h>
#include <gnuradio/io_signature.h>
#include <gnuradio/types.h>
#include <iostream>
#include <stdio.h>

#include "filter/decimator.h"
#include "hbf_decim.h"


hbf_decim_sptr make_hbf_decim(unsigned int decim)
{
    return gnuradio::get_initial_sptr (new hbf_decim(decim));
}

hbf_decim::hbf_decim(unsigned int decim)
  : gr::sync_decimator("hbf_decim",
          gr::io_signature::make(1, 1, sizeof(gr_complex)),
          gr::io_signature::make(1, 1, sizeof(gr_complex)), decim)
{
    decimation = decim;
    dec = new Decimator();
    if (dec->init(decim, 100) != decim)
        throw std::range_error("Decimation not supported");

    std::cout << "New decimator: " << decimation << std::endl;
}

hbf_decim::~hbf_decim()
{
    delete dec;
}

int hbf_decim::work(int noutput_items,
          gr_vector_const_void_star &input_items,
          gr_vector_void_star &output_items)
{
    gr_complex *in = (gr_complex *) input_items[0];
    gr_complex *out = (gr_complex *) output_items[0];

    /* HBF input length must be even and >= number of taps.
     * Max taps is 87 => we need at least 44 * decim input samples
     */
    if (noutput_items < 44)
        return 0;

    return dec->process(noutput_items * decimation, in, out);
}


