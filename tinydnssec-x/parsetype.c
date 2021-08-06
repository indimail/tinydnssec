#include "scan.h"
#include "byte.h"
#include "case.h"
#include "dns.h"
#include "uint16.h"
#include "parsetype.h"

int parsetype(char *s,char type[2])
{
  unsigned long u;

  if (!s[scan_ulong(s,&u)]) uint16_pack_big(type,u);
  else if (case_equals(s,"any")) byte_copy(type,2,DNS_T_ANY);
  else if (case_equals(s,"a")) byte_copy(type,2,DNS_T_A);
  else if (case_equals(s,"ns")) byte_copy(type,2,DNS_T_NS);
  else if (case_equals(s,"mx")) byte_copy(type,2,DNS_T_MX);
  else if (case_equals(s,"ptr")) byte_copy(type,2,DNS_T_PTR);
  else if (case_equals(s,"txt")) byte_copy(type,2,DNS_T_TXT);
  else if (case_equals(s,"cname")) byte_copy(type,2,DNS_T_CNAME);
  else if (case_equals(s,"soa")) byte_copy(type,2,DNS_T_SOA);
  else if (case_equals(s,"hinfo")) byte_copy(type,2,DNS_T_HINFO);
  else if (case_equals(s,"rp")) byte_copy(type,2,DNS_T_RP);
  else if (case_equals(s,"sig")) byte_copy(type,2,DNS_T_SIG);
  else if (case_equals(s,"key")) byte_copy(type,2,DNS_T_KEY);
  else if (case_equals(s,"aaaa")) byte_copy(type,2,DNS_T_AAAA);
  else if (case_equals(s,"axfr")) byte_copy(type,2,DNS_T_AXFR);
#ifdef DNSSEC
  else if (case_equals(s,"naptr")) byte_copy(type,2,DNS_T_NAPTR);
  else if (case_equals(s,"cert")) byte_copy(type,2,DNS_T_CERT);
  else if (case_equals(s,"opt")) byte_copy(type,2,DNS_T_OPT);
  else if (case_equals(s,"ds")) byte_copy(type,2,DNS_T_DS);
  else if (case_equals(s,"srv")) byte_copy(type,2,DNS_T_SRV);
  else if (case_equals(s,"sshfp")) byte_copy(type,2,DNS_T_SSHFP);
  else if (case_equals(s,"ipseckey")) byte_copy(type,2,DNS_T_IPSECKEY);
  else if (case_equals(s,"rrsig")) byte_copy(type,2,DNS_T_RRSIG);
  else if (case_equals(s,"nsec")) byte_copy(type,2,DNS_T_NSEC);
  else if (case_equals(s,"dnskey")) byte_copy(type,2,DNS_T_DNSKEY);
  else if (case_equals(s,"nsec3")) byte_copy(type,2,DNS_T_NSEC3);
  else if (case_equals(s,"nsec3param")) byte_copy(type,2,DNS_T_NSEC3PARAM);
  else if (case_equals(s,"tlsa")) byte_copy(type,2,DNS_T_TLSA);
  else if (case_equals(s,"hip")) byte_copy(type,2,DNS_T_HIP);
  else if (case_equals(s,"openpgpkey")) byte_copy(type,2,DNS_T_OPENPGPKEY);
  else if (case_equals(s,"spf")) byte_copy(type,2,DNS_T_SPF);
  else if (case_equals(s,"caa")) byte_copy(type,2,DNS_T_CAA);
#endif
  else
    return 0;

  return 1;
}
