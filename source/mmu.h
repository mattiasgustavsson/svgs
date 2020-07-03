#ifndef mmu_h
#define mmu_h

typedef struct mmu_t mmu_t;

mmu_t* mmu_create( void* ram, int ram_size );
void mmu_destroy( mmu_t* mmu );

uint8_t mmu_read_byte( mmu_t* mmu, uint32_t address );
uint16_t mmu_read_word( mmu_t* mmu, uint32_t address );
uint32_t mmu_read_long( mmu_t* mmu, uint32_t address );
float mmu_read_float( mmu_t* mmu, uint32_t address );

void mmu_write_byte( mmu_t* mmu, uint32_t address, uint8_t value );
void mmu_write_word( mmu_t* mmu, uint32_t address, uint16_t value );
void mmu_write_long( mmu_t* mmu, uint32_t address, uint32_t value );
void mmu_write_float( mmu_t* mmu, uint32_t address, float value );

#endif /* mmu_h */


#ifdef SVGS_IMPLEMENTATION


struct mmu_t {
    void* ram;
    int ram_size;

};


mmu_t* mmu_create( void* ram, int ram_size ) {
    mmu_t* mmu = (mmu_t*) malloc( sizeof( mmu_t ) );
    memset( mmu, 0, sizeof( *mmu ) );

    mmu->ram = ram;
    mmu->ram_size = ram_size;
    return mmu;
}


void mmu_destroy( mmu_t* mmu ) {
    free( mmu );
}


INLINE void* mmu_ram_ptr( mmu_t* mmu, uint32_t address ) {
    return (void*)( ((uintptr_t)mmu->ram) + address );
}


uint8_t mmu_read_byte( mmu_t* mmu, uint32_t address ) { 
    return *(uint8_t*)mmu_ram_ptr( mmu, address ); 
}


uint16_t mmu_read_word( mmu_t* mmu, uint32_t address ) { 
    return *(uint16_t*)mmu_ram_ptr( mmu, address ); 
}


uint32_t mmu_read_long( mmu_t* mmu, uint32_t address ) { 
    return *(uint32_t*)mmu_ram_ptr( mmu, address ); 
}


float mmu_read_float( mmu_t* mmu, uint32_t address ) { 
    return *(float*)mmu_ram_ptr( mmu, address ); 
}


void mmu_write_byte( mmu_t* mmu, uint32_t address, uint8_t value ) { 
    *(uint8_t*)mmu_ram_ptr( mmu, address ) = value; 
}


void mmu_write_word( mmu_t* mmu, uint32_t address, uint16_t value ) { 
    *(uint16_t*)mmu_ram_ptr( mmu, address ) = value; 
}


void mmu_write_long( mmu_t* mmu, uint32_t address, uint32_t value ) { 
    *(uint32_t*)mmu_ram_ptr( mmu, address ) = value; 
}


void mmu_write_float( mmu_t* mmu, uint32_t address, float value ) { 
    *(float*)mmu_ram_ptr( mmu, address ) = value; 
}


#endif /* SVGS_IMPLEMENTATION */