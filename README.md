# pfsvm_blk_grad
It is expected that local characteristics of the reconstruction errors depend not only on a relative position within the TU but also on a direction of the TU boundary. Therefore, arrangement of pels used for the feature vector is rotated at the TU boundaries so that the block boundary is always crossing between two pels p_0 and p_1. Since the direction has two candidates at a corner of TU, gradients of the reconstructed image across the block boundaries are calculated ot determine one of them.