/*
 *  ******************************************************************************
 *  *
 *  *
 *  * This program and the accompanying materials are made available under the
 *  * terms of the Apache License, Version 2.0 which is available at
 *  * https://www.apache.org/licenses/LICENSE-2.0.
 *  *
 *  *  See the NOTICE file distributed with this work for additional
 *  *  information regarding copyright ownership.
 *  * Unless required by applicable law or agreed to in writing, software
 *  * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 *  * License for the specific language governing permissions and limitations
 *  * under the License.
 *  *
 *  * SPDX-License-Identifier: Apache-2.0
 *  *****************************************************************************
 */

package org.deeplearning4j.nn.layers.convolution;

import lombok.val;
import org.deeplearning4j.nn.api.Layer;
import org.deeplearning4j.nn.conf.NeuralNetConfiguration;
import org.deeplearning4j.nn.gradient.DefaultGradient;
import org.deeplearning4j.nn.gradient.Gradient;
import org.deeplearning4j.nn.layers.AbstractLayer;
import org.deeplearning4j.nn.workspace.ArrayType;
import org.deeplearning4j.nn.workspace.LayerWorkspaceMgr;
import org.nd4j.linalg.api.buffer.DataType;
import org.nd4j.linalg.api.ndarray.INDArray;
import org.nd4j.common.primitives.Pair;

import static org.nd4j.linalg.indexing.NDArrayIndex.all;
import static org.nd4j.linalg.indexing.NDArrayIndex.interval;

public class Cropping3DLayer extends AbstractLayer<org.deeplearning4j.nn.conf.layers.convolutional.Cropping3D> {

    private int[] cropping; //[cropLeftD, cropRightD, cropLeftH, cropRightH, cropLeftW, cropRightW]

    public Cropping3DLayer(NeuralNetConfiguration conf, DataType dataType) {
        super(conf, dataType);
        this.cropping = ((org.deeplearning4j.nn.conf.layers.convolutional.Cropping3D) conf.getLayer()).getCropping();
    }

    @Override
    public boolean isPretrainLayer() {
        return false;
    }

    @Override
    public void clearNoiseWeightParams() {
        //No op
    }

    @Override
    public Type type() {
        return Type.CONVOLUTIONAL3D;
    }

    @Override
    public Pair<Gradient, INDArray> backpropGradient(INDArray epsilon, LayerWorkspaceMgr workspaceMgr) {
        val inShape = input.shape();
        INDArray epsNext = workspaceMgr.create(ArrayType.ACTIVATION_GRAD, input.dataType(), inShape, 'c');
        INDArray epsNextSubset = inputSubset(epsNext);
        epsNextSubset.assign(epsilon);
        return new Pair<>(new DefaultGradient(), workspaceMgr.leverageTo(ArrayType.ACTIVATION_GRAD,epsNext));
    }


    @Override
    public INDArray activate(boolean training, LayerWorkspaceMgr workspaceMgr) {
        assertInputSet(false);
        INDArray ret = inputSubset(input);
        ret = workspaceMgr.leverageTo(ArrayType.ACTIVATIONS, ret);
        workspaceMgr.validateArrayLocation(ArrayType.ACTIVATIONS, ret, false, false);
        return ret;
    }

    @Override
    public Layer clone() {
        return new Cropping3DLayer(conf.clone(), dataType);
    }

    @Override
    public double calcRegularizationScore(boolean backpropParamsOnly){
        return 0;
    }

    private INDArray inputSubset(INDArray from){
        //NCDHW format
        return from.get(all(), all(),
                interval(cropping[0], from.size(2)-cropping[1]),
                interval(cropping[2], from.size(3)-cropping[3]),
                interval(cropping[4], from.size(4)-cropping[5]));
    }
}
