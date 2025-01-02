#include <msp430.h>
#include <stdio.h>
#include <sancus/sm_support.h>
#include <sancus/reactive.h>
#include <sancus_support/sm_io.h>

#define CONN_ID 0
#define AD_SIZE 6
// NOTE: because of a known bug in the Sancus crypto engine, manually align buffers here
uint8_t __attribute__((aligned(2))) cipher[SANCUS_KEY_SIZE];
uint8_t __attribute__((aligned(2))) tag[SANCUS_TAG_SIZE];
uint8_t __attribute__((aligned(2))) ad[AD_SIZE] = {0x0};
uint8_t __attribute__((aligned(2))) key[SANCUS_KEY_SIZE] = 
    #if SANCUS_KEY_SIZE == 8
        { 0x83, 0x53, 0x6b, 0xf3, 0x07, 0x1f, 0xec, 0x8b };
    #else
        { 0x83, 0x53, 0x6b, 0xf3, 0x07, 0x1f, 0xec, 0x8b, 0xc1, 0x7c, 0xb2, 0x16, 0xca, 0xc5, 0x9a, 0x5e};
    #endif

/* ********************************************************************** */
DECLARE_SM(ping, 0x1234);
SM_OUTPUT(ping, ping_produce_output);
uint16_t SM_ENTRY(ping) __sm_ping_set_key(const uint8_t* ad,
                            const uint8_t* cipher, const uint8_t* tag, uint16_t *conn_idx);

void SM_ENTRY(ping) ping_gen_key(void)
{
    sancus_wrap(ad, AD_SIZE, key, SANCUS_KEY_SIZE, cipher, tag);
}

void SM_ENTRY(ping) enter_ping(void)
{
    uint16_t data = 0xAABB;
    puts("[ping] producing auth output..");
    ASSERT( ping_produce_output((uint8_t*) &data, sizeof(data)) == 0);
}

/* ********************************************************************** */
DECLARE_SM(pong, 0x1234);

uint16_t SM_ENTRY(pong) __sm_pong_set_key(const uint8_t* ad,
                            const uint8_t* cipher, const uint8_t* tag, uint16_t *conn_idx);

void SM_ENTRY(pong) pong_gen_key(void)
{
    sancus_wrap(ad, AD_SIZE, key, SANCUS_KEY_SIZE, cipher, tag);
}

SM_INPUT(pong, pong_process_input, data, len)
{
    puts("[pong] hi with auth input:");
    dump_buf(data, len, "\tdata");
}

uint16_t SM_ENTRY(pong) __sm_pong_handle_input(uint16_t conn_idx,
                                         const void* payload, size_t len);

/* ********************************************************************** */

void reactive_handle_output(conn_index conn_id, void* data, size_t len)
{
    /* dummy implementation: since there's only one connection, we don't need
     * to do any routing and can simply hardcode the forwarding here */
    puts("[untrusted] forwarding auth output from ping to pong..");
    dump_buf(data, len, "\tdata");
    int rv = __sm_pong_handle_input(CONN_ID, data, len);
    printf("[untrusted] pong returned %d\n", rv);
    ASSERT( rv == 0);
}

int main()
{
    uint16_t rv_idx = -1;
    msp430_io_init();

    puts("[untrusted] enabling ping/pong SMs..");
    sancus_enable(&ping);
    pr_sm_info(&ping);
    sancus_enable(&pong);
    pr_sm_info(&pong);

    puts("\n---");
    puts("[untrusted] setting SM keys..");
    ping_gen_key();
    dump_buf(key, SANCUS_KEY_SIZE, "\tkey");
    dump_buf(ad, AD_SIZE, "\tad");
    dump_buf(cipher, SANCUS_KEY_SIZE, "\tcipher");
    dump_buf(tag, SANCUS_TAG_SIZE, "\ttag");
    ASSERT( __sm_ping_set_key(ad, cipher, tag, &rv_idx) == 0);
    printf("[untrusted] connection idx is %d\n", rv_idx);
    ASSERT( rv_idx == CONN_ID);

    pong_gen_key();
    dump_buf(key, SANCUS_KEY_SIZE, "\tkey");
    dump_buf(ad, AD_SIZE, "\tad");
    dump_buf(cipher, SANCUS_KEY_SIZE, "\tcipher");
    dump_buf(tag, SANCUS_TAG_SIZE, "\ttag");
    ASSERT( __sm_pong_set_key(ad, cipher, tag, &rv_idx) == 0);
    printf("[untrusted] connection idx is %d\n", rv_idx);
    ASSERT( rv_idx == CONN_ID);

    puts("\n---");
    puts("[untrusted] calling ping SM..");
    enter_ping();

    puts("[untrusted] exiting..");
    EXIT();
}
