#ifndef CHUNK_TO_JSON_CONVERTER
#define CHUNK_TO_JSON_CONVERTER

/* Library Includes */
#include "json.hpp"                     // Used for JSON documents
#include "ChunkTypesNamingUtility.h"    // Used When filling out json documents to display Chunk Type

// Abstract class that enforces a ToJSON conversion for a chunk
class ChunkToJSONConverter {
public:

    // Virtual destructor (necessary for polymorphism)
    virtual ~ChunkToJSONConverter() {};

    // Returns the JSON equivalent of this classes representation
    virtual std::shared_ptr<nlohmann::json> ToJSON() = 0;
};

#endif
