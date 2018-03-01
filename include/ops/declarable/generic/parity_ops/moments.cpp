//
// Created by george@skymind.io on 26.01.2018.
//

#include <ops/declarable/CustomOperations.h>

namespace nd4j {
    namespace ops {
        CUSTOM_OP_IMPL(moments, 1, 2, false, 0, -2) {
            NDArray<T>* input = INPUT_VARIABLE(0);
            NDArray<T>* means = OUTPUT_VARIABLE(0);
            NDArray<T>* variances = OUTPUT_VARIABLE(1);

            std::vector<int> axis = *block.getIArguments();

            // axis might be dynamic (i.e. tf mode)
            if (block.width() > 1 && axis.size() == 0) {
                auto axisVector = INPUT_VARIABLE(1);

                for (int e = 0; e < axisVector->lengthOf(); e++) {
                    int ca = (int) axisVector->getScalar(e);
                    if (ca < 0)
                        ca += input->rankOf();

                    axis.emplace_back(ca);
                }

            }

            std::vector<int>& dims = axis;
            input->template varianceAlongDimension<simdOps::SummaryStatsVariance<T>>(variances, false, axis);
            input->template reduceAlongDimension<simdOps::Mean<T>>(means, axis);

            return ND4J_STATUS_OK;
        }

        DECLARE_SHAPE_FN(moments) {
            std::vector<int> axis = *block.getIArguments();
            auto input = INPUT_VARIABLE(0);

            // axis might be dynamic (i.e. tf mode)
            if (block.width() > 1 && axis.size() == 0) {
                auto axisVector = INPUT_VARIABLE(1);

                for (int e = 0; e < axisVector->lengthOf(); e++) {
                    int ca = (int) axisVector->getScalar(e);
                    if (ca < 0)
                        ca += input->rankOf();

                    axis.emplace_back(ca);
                }

            }
            //std::vector<int> dims = ShapeUtils<T>::convertAxisToTadTarget(input->rankOf(), {axis});

            int* meanShape = ShapeUtils<T>::evalReduceShapeInfo('c', axis, *input, false, false, block.workspace());
            int* varianceShape = ShapeUtils<T>::evalReduceShapeInfo('c', axis, *input, false, false, block.workspace());
            auto shapeList = SHAPELIST(); 
            shapeList->push_back(meanShape);
            shapeList->push_back(varianceShape);

            return shapeList;
        }
    }

}
