#ifndef jpeg_ourex_jpeg_h
#define jpeg_ourex_jpeg_h

#define jpeg_CreateCompress ourex_jpeg_CreateCompress
#define jpeg_CreateDecompress ourex_jpeg_CreateDecompress
#define jpeg_abort_decompress ourex_jpeg_abort_decompress
#define jpeg_destroy_compress ourex_jpeg_destroy_compress
#define jpeg_destroy_decompress ourex_jpeg_destroy_decompress
#define jpeg_finish_compress ourex_jpeg_finish_compress
#define jpeg_finish_decompress ourex_jpeg_finish_decompress
#define jpeg_read_header ourex_jpeg_read_header
#define jpeg_read_scanlines ourex_jpeg_read_scanlines
#define jpeg_set_defaults ourex_jpeg_set_defaults
#define jpeg_set_quality ourex_jpeg_set_quality
#define jpeg_start_compress ourex_jpeg_start_compress
#define jpeg_start_decompress ourex_jpeg_start_decompress
#define jpeg_std_error ourex_jpeg_std_error
#define jpeg_stdio_dest ourex_jpeg_stdio_dest
#define jpeg_stdio_src ourex_jpeg_stdio_src
#define jpeg_write_scanlines ourex_jpeg_write_scanlines

#define jpeg_abort ourex_jpeg_abort
#define jpeg_abort_compress ourex_jpeg_abort_compress
#define jpeg_alloc_huff_table ourex_jpeg_alloc_huff_table
#define jpeg_alloc_quant_table ourex_jpeg_alloc_quant_table
/*
#define jpeg_add_quant_table ourex_jpeg_add_quant_table
#define jpeg_calc_output_dimensions ourex_jpeg_calc_output_dimensions
*/
#define jpeg_consume_input ourex_jpeg_consume_input
#define jpeg_copy_critical_parameters ourex_jpeg_copy_critical_parameters
#define jpeg_default_colorspace ourex_jpeg_default_colorspace
#define jpeg_destroy ourex_jpeg_destroy
#define jpeg_fdct_float ourex_jpeg_fdct_float
#define jpeg_fdct_ifast ourex_jpeg_fdct_ifast
#define jpeg_fdct_islow ourex_jpeg_fdct_islow
#define jpeg_fill_bit_buffer ourex_jpeg_fill_bit_buffer
#define jpeg_finish_output ourex_jpeg_finish_output
#define jpeg_free_large ourex_jpeg_free_large
#define jpeg_free_small ourex_jpeg_free_small
#define jpeg_gen_optimal_table ourex_jpeg_gen_optimal_table
#define jpeg_get_large ourex_jpeg_get_large
#define jpeg_get_small ourex_jpeg_get_small
#define jpeg_has_multiple_scans ourex_jpeg_has_multiple_scans
#define jpeg_huff_decode ourex_jpeg_huff_decode
#define jpeg_idct_1x1 ourex_jpeg_idct_1x1
#define jpeg_idct_2x2 ourex_jpeg_idct_2x2
#define jpeg_idct_4x4 ourex_jpeg_idct_4x4
#define jpeg_idct_float ourex_jpeg_idct_float
#define jpeg_idct_ifast ourex_jpeg_idct_ifast
#define jpeg_idct_islow ourex_jpeg_idct_islow
#define jpeg_input_complete ourex_jpeg_input_complete
#define jpeg_make_c_derived_tbl ourex_jpeg_make_c_derived_tbl
#define jpeg_make_d_derived_tbl ourex_jpeg_make_d_derived_tbl
#define jpeg_mem_available ourex_jpeg_mem_available
#define jpeg_mem_init ourex_jpeg_mem_init
#define jpeg_mem_term ourex_jpeg_mem_term
#define jpeg_natural_order ourex_jpeg_natural_order
#define jpeg_new_colormap ourex_jpeg_new_colormap
#define jpeg_open_backing_store ourex_jpeg_open_backing_store
#define jpeg_quality_scaling ourex_jpeg_quality_scaling
#define jpeg_rdrle_dummy ourex_jpeg_rdrle_dummy
#define jpeg_read_coefficients ourex_jpeg_read_coefficients
#define jpeg_read_raw_data ourex_jpeg_read_raw_data
#define jpeg_resync_to_restart ourex_jpeg_resync_to_restart
#define jpeg_save_markers ourex_jpeg_save_markers
#define jpeg_set_colorspace ourex_jpeg_set_colorspace
#define jpeg_set_linear_quality ourex_jpeg_set_linear_quality
#define jpeg_set_marker_processor ourex_jpeg_set_marker_processor
#define jpeg_simple_progression ourex_jpeg_simple_progression
#define jpeg_start_output ourex_jpeg_start_output
#define jpeg_std_message_table ourex_jpeg_std_message_table
#define jpeg_suppress_tables ourex_jpeg_suppress_tables
#define jpeg_write_coefficients ourex_jpeg_write_coefficients
#define jpeg_write_m_byte ourex_jpeg_write_m_byte
#define jpeg_write_m_header ourex_jpeg_write_m_header
#define jpeg_write_marker ourex_jpeg_write_marker
#define jpeg_write_raw_data ourex_jpeg_write_raw_data
#define jpeg_write_tables ourex_jpeg_write_tables
#define jpeg_wrrle_dummy ourex_jpeg_wrrle_dummy

#define jinit_1pass_quantizer             ourex_jpeg_jinit_1pass_quantizer
#define jinit_2pass_quantizer             ourex_jpeg_jinit_2pass_quantizer
#define jinit_c_coef_controller           ourex_jpeg_jinit_c_coef_controller
#define jinit_c_main_controller           ourex_jpeg_jinit_c_main_controller
#define jinit_c_master_control            ourex_jpeg_jinit_c_master_control
#define jinit_c_prep_controller           ourex_jpeg_jinit_c_prep_controller
#define jinit_color_converter             ourex_jpeg_jinit_color_converter
#define jinit_color_deconverter           ourex_jpeg_jinit_color_deconverter
#define jinit_compress_master             ourex_jpeg_jinit_compress_master
#define jinit_d_coef_controller           ourex_jpeg_jinit_d_coef_controller
#define jinit_d_main_controller           ourex_jpeg_jinit_d_main_controller
#define jinit_d_post_controller           ourex_jpeg_jinit_d_post_controller
#define jinit_downsampler                 ourex_jpeg_jinit_downsampler
#define jinit_forward_dct                 ourex_jpeg_jinit_forward_dct
#define jinit_huff_decoder                ourex_jpeg_jinit_huff_decoder
#define jinit_huff_encoder                ourex_jpeg_jinit_huff_encoder
#define jinit_input_controller            ourex_jpeg_jinit_input_controller
#define jinit_inverse_dct                 ourex_jpeg_jinit_inverse_dct
#define jinit_marker_reader               ourex_jpeg_jinit_marker_reader
#define jinit_marker_writer               ourex_jpeg_jinit_marker_writer
#define jinit_master_decompress           ourex_jpeg_jinit_master_decompress
#define jinit_memory_mgr                  ourex_jpeg_jinit_memory_mgr
#define jinit_merged_upsampler            ourex_jpeg_jinit_merged_upsampler
#define jinit_phuff_decoder               ourex_jpeg_jinit_phuff_decoder
#define jinit_phuff_encoder               ourex_jpeg_jinit_phuff_encoder
#define jinit_read_bmp ourex_jpeg_jinit_read_bmp
#define jinit_read_gif ourex_jpeg_jinit_read_gif
#define jinit_read_ppm ourex_jpeg_jinit_read_ppm
#define jinit_read_targa ourex_jpeg_jinit_read_targa
#define jinit_upsampler ourex_jpeg_jinit_upsampler
#define jinit_write_bmp ourex_jpeg_jinit_write_bmp
#define jinit_write_gif ourex_jpeg_jinit_write_gif
#define jinit_write_ppm ourex_jpeg_jinit_write_ppm
#define jinit_write_targa ourex_jpeg_jinit_write_targa

#define jcopy_block_row                   ourex_jpeg_jcopy_block_row
#define jcopy_markers_execute             ourex_jpeg_jcopy_markers_execute
#define jcopy_markers_setup               ourex_jpeg_jcopy_markers_setup
#define jcopy_sample_rows                 ourex_jpeg_jcopy_sample_rows

#define jdiv_round_up                     ourex_jpeg_jdiv_round_up
#define jround_up ourex_jpeg_jround_up
#define jtransform_adjust_parameters ourex_jpeg_jtransform_adjust_parameters
#define jtransform_execute_transformation ourex_jpeg_jtransform_execute_transformation
#define jtransform_request_workspace ourex_jpeg_jtransform_request_workspace
#define jzero_far ourex_jpeg_jzero_far

#define read_color_map ourex_jpeg_read_color_map
#define read_quant_tables ourex_jpeg_read_quant_tables
#define read_scan_script ourex_jpeg_read_scan_script
#define read_stdin ourex_jpeg_read_stdin

#define set_quant_slots ourex_jpeg_set_quant_slots
#define set_sample_factors ourex_jpeg_set_sample_factors

#define keymatch ourex_jpeg_keymatch
#define write_stdout ourex_jpeg_write_stdout

#include "_jpeglib.h"

#endif
