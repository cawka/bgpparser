
#ifndef _PARAMS_H_
#define _PARAMS_H_

struct Info
{
	std::string mrt_type;
	uint32_t	timestamp;
	std::string dump_type; /* B|A|W */
	IPAddress   peer_addr;
	uint8_t		peer_afi;
	uint32_t	peer_as;

	std::string as_path    ;
	std::string origin     ;
	std::string next_hop   ;
	uint32_t    med        ;
	uint32_t    lpref      ;
	std::string atomic_agg ;
	std::string agg		   ;
	std::string communities;

	uint8_t		afi;
	bool		parseNLRI;

	Info( const std::string &_mrt_type,  uint32_t _timestamp,
		  const std::string &_dump_type,
		  const IPAddress   &_peer_addr, uint8_t _peer_afi, uint32_t _peer_as )
	: mrt_type(_mrt_type), timestamp(_timestamp), dump_type(_dump_type),
	  peer_addr(_peer_addr), peer_as(_peer_as), peer_afi(_peer_afi),
	  med(0), lpref(0), atomic_agg("NAG"), parseNLRI( false )
	{ }
};

typedef boost::shared_ptr<Info>	InfoPtr;

#endif
