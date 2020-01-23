typedef struct Dvec dvec_t;

dvec_t *DVECCreate(size_t size_of_element,size_t num_elements);
void DVECDestroy(dvec_t *dvec);
void *DVECGetItemAddress(dvec_t *dvec, size_t index);
/* returns 1 on allocation failure */
int DVECPushBack(dvec_t *dvec, const void *element);
void DVECPopBack(dvec_t *dvec);
size_t DVECSize(const dvec_t *dvec);
size_t DVECCapacity(const dvec_t *dvec);
/* returns 1 on allocation failure */
int DVECReserve(dvec_t *dvec, size_t new_capacity);

