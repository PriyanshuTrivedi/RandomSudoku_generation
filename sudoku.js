const n=9;
const totalToRemove=55;
let completeSudoku=[],uniqueSolvableSudoku=[];
let multiSolutionExits,solnExits;

const findRandomNumberBtw=(a,b)=>{
    return a+Math.floor((b-a+1)*Math.random());
}

const swapElsOfArr=(arr,posa,posb)=>{
    let temp=arr[posa];
    arr[posa]=arr[posb];
    arr[posb]=temp;
}

const genRandom9Perm=()=>{
    const perm=[];
    let i;
    for(i=1;i<=n;i++){
        perm.push(i);
    }
    for(i=0;i<n;i++){
        swapElsOfArr(perm,i,findRandomNumberBtw(0,n-1));
    }
    return perm;
}

const makeCopyof2DArr=(arr2D)=>{
    const result=[];
    let i,j;
    for(i=0;i<n;i++){
        let tempArr=[];
        for(j=0;j<n;j++){
            tempArr.push(arr2D[i][j]);
        }
        result.push(tempArr);
    }
    return result;
}

const initialPartiallyFilledSudoku=(possDigsForBoxij)=>{
    const initialSudoku=[];
    let i,j,k;
    for(i=0;i<n;i++){
        const tempArr=[];
        for(j=0;j<n;j++){
            tempArr.push(0);
        }
        initialSudoku.push(tempArr);
    }
    for(k=0;k<=6;k+=3){
        const tempPerm=genRandom9Perm();
        let indx=0;
        for(i=k;i<k+3;i++){
            for(j=k;j<k+3;j++){
                possDigsForBoxij[i*n+j].clear();
                initialSudoku[i][j]=tempPerm[indx];
                let tempArr=[];
                updateRow(i,tempPerm[indx],possDigsForBoxij,tempArr);
                updateColumn(j,tempPerm[indx],possDigsForBoxij,tempArr);
                indx++;
            }
        }
    }
    return initialSudoku;
}

const initialPossDigsForBoxij=()=>{
    const res=[];
    let i,j;
    for(i=0;i<n*n;i++){
        const tempSet=new Set();
        for(j=1;j<=n;j++)
        tempSet.add(j);
        res.push(tempSet);
    }
    return res;
}

const console2DArray=(arr2D)=>{
    if(arr2D.length===0){
        console.log('n bhai glti hori');
    }
    let i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            process.stdout.write(arr2D[i][j]+' ');
        }
        console.log();
    }
}

const permute=(arr)=>{
    let i,len;
    len=arr.length;
    for(i=0;i<len;i++){
        swapElsOfArr(arr,i,findRandomNumberBtw(0,len-1));
    }
}

const findRandomIndexWithMinPossibleDigs=(possDigsForBoxij)=>{
    let k;
    const temp2DArr=[];
    for(k=0;k<=n;k++){
        let tempArr=[];
        temp2DArr.push(tempArr);
    }
    for(k=0;k<n*n;k++){
        temp2DArr[possDigsForBoxij[k].size].push(k);
    }
    for(k=1;k<=n;k++){
        if(temp2DArr[k].length>0){
            const res=temp2DArr[k][findRandomNumberBtw(0,temp2DArr[k].length-1)];
            const i=Math.floor(res/n);
            const j=res%n;
            return {
                i:i,
                j:j
            };
        }
    }
    return {
        i:-1,
        j:-1
    };
}

const updateRow=(i,possVal,possDigsForBoxij,boxesAffectedInRow)=>{
    let j;
    for(j=0;j<n;j++){
        if(possDigsForBoxij[i*n+j].has(possVal)){
            possDigsForBoxij[i*n+j].delete(possVal);
            boxesAffectedInRow.push(j);
        }
    }
}

const updateColumn=(j,possVal,possDigsForBoxij,boxesAffectedInColumn)=>{
    let i;
    for(i=0;i<n;i++){
        if(possDigsForBoxij[i*n+j].has(possVal)){
            possDigsForBoxij[i*n+j].delete(possVal);
            boxesAffectedInColumn.push(i);
        }
    }
}

const update3X3Box=(i,j,possVal,possDigsForBoxij,boxesAffectedIn3X3Box)=>{
    let sti,stj,ii,jj;
    sti=Math.floor(i/3)*3;
    stj=Math.floor(j/3)*3;
    for(ii=sti;ii<sti+3;ii++){
        for(jj=stj;jj<stj+3;jj++){
            if(possDigsForBoxij[ii*n+jj].has(possVal)){
                possDigsForBoxij[ii*n+jj].delete(possVal);
                boxesAffectedIn3X3Box.push(ii*n+jj);
            }
        }
    }
}

const updateBox=(i,j,possVal,tempSudoku,possDigsForBoxij)=>{
    tempSudoku[i][j]=possVal;
    possDigsForBoxij[i*n+j].clear();
}

const update=(i,j,possVal,tempSudoku,possDigsForBoxij,boxesAffectedIn3X3Box,boxesAffectedInRow,boxesAffectedInColumn)=>{
    updateBox(i,j,possVal,tempSudoku,possDigsForBoxij);
    updateRow(i,possVal,possDigsForBoxij,boxesAffectedInRow);
    updateColumn(j,possVal,possDigsForBoxij,boxesAffectedInColumn);
    update3X3Box(i,j,possVal,possDigsForBoxij,boxesAffectedIn3X3Box);
}

const undoUpdatesInBox=(i,j,prevPossDigs,tempSudoku,possDigsForBoxij)=>{
    tempSudoku[i][j]=0;
    prevPossDigs.forEach(el => {
        possDigsForBoxij[i*n+j].add(el);
    });
}

const undoUpdatesInRow=(i,possVal,possDigsForBoxij,boxesAffectedInRow)=>{
    boxesAffectedInRow.forEach(columnNo => {
        possDigsForBoxij[i*n+columnNo].add(possVal);
    });
}

const undoUpdatesInColumn=(j,possVal,possDigsForBoxij,boxesAffectedInColumn)=>{
    boxesAffectedInColumn.forEach(rowNo => {
        possDigsForBoxij[rowNo*n+j].add(possVal);
    });
}

const undoUpdatesIn3X3Box=(possVal,possDigsForBoxij,boxesAffectedIn3X3Box)=>{
   boxesAffectedIn3X3Box.forEach(box => {
        possDigsForBoxij[box].add(possVal);
   });
}

const undoUpdates=(i,j,prevPossDigs,possVal,tempSudoku,possDigsForBoxij,boxesAffectedIn3X3Box,boxesAffectedInRow,boxesAffectedInColumn)=>{
    undoUpdatesInBox(i,j,prevPossDigs,tempSudoku,possDigsForBoxij);
    undoUpdatesInRow(i,possVal,possDigsForBoxij,boxesAffectedInRow);
    undoUpdatesInColumn(j,possVal,possDigsForBoxij,boxesAffectedInColumn);
    undoUpdatesIn3X3Box(possVal,possDigsForBoxij,boxesAffectedIn3X3Box);
}

const generateSudoku=(tempSudoku,possDigsForBoxij,blanks)=>{
    if(blanks===0){
        completeSudoku=makeCopyof2DArr(tempSudoku);
        return true;
    }
    const {i,j}=findRandomIndexWithMinPossibleDigs(possDigsForBoxij);
    if(i===-1){
        return false;
    }
    const tempSet=new Set(possDigsForBoxij[i*n+j]);
    for(const possVal of tempSet){
        const boxesAffectedInRow=[],boxesAffectedInColumn=[],boxesAffectedIn3X3Box=[],prevPossDigs=new Set(tempSet);
        update(i,j,possVal,tempSudoku,possDigsForBoxij,boxesAffectedIn3X3Box,boxesAffectedInRow,boxesAffectedInColumn);
        if(generateSudoku(tempSudoku,possDigsForBoxij,blanks-1)===true){
            return true;
        }
        undoUpdates(i,j,prevPossDigs,possVal,tempSudoku,possDigsForBoxij,boxesAffectedIn3X3Box,boxesAffectedInRow,boxesAffectedInColumn);
    };
    return false;
}

const initializeCanRemove=()=>{
    const canRemove=[];
    for(let i=0;i<n*n;i++){
        canRemove.push(i);
    }
    return canRemove;
}

const randomSwapLastEl=(arr)=>{
    let sizeOfArr=arr.length;
    swapElsOfArr(arr,sizeOfArr-1,findRandomNumberBtw(0,sizeOfArr-1));
}

const multiSolnExits=(tempSudoku,possDigsForBoxij,blanks)=>{
    if(blanks===0){
        if(solnExits===true){
            multiSolutionExits=true;
        }
        solnExits=true;
        return;
    }
    const {i,j}=findRandomIndexWithMinPossibleDigs(possDigsForBoxij);
    if(i===-1){
        return;
    }
    const tempSet=new Set(possDigsForBoxij[i*n+j]);
    for(const possVal of tempSet){
        const boxesAffectedInRow=[],boxesAffectedInColumn=[],boxesAffectedIn3X3Box=[],prevPossDigs=new Set(tempSet);
        update(i,j,possVal,tempSudoku,possDigsForBoxij,boxesAffectedIn3X3Box,boxesAffectedInRow,boxesAffectedInColumn);
        multiSolnExits(tempSudoku,possDigsForBoxij,blanks-1);
        if(multiSolutionExits===true){
            return;
        }
        undoUpdates(i,j,prevPossDigs,possVal,tempSudoku,possDigsForBoxij,boxesAffectedIn3X3Box,boxesAffectedInRow,boxesAffectedInColumn);
    };
}

const findPossDigitsForBoxij=(tempSudoku)=>{
    const res=[];
    let i,j,k,l;
    for(i=0;i<n*n;i++){
        const tempSet=new Set();
        for(j=1;j<=n;j++){
            tempSet.add(j);
        }
        res.push(tempSet);
    }
    const tempArr=[];
    for(i=0;i<n*n;i++){
        k=Math.floor(i/n);
        l=i%n;
        if(tempSudoku[k][l]>0){
            res[i].clear();
            update3X3Box(k,l,tempSudoku[k][l],res,tempArr);
            updateRow(k,tempSudoku[k][l],res,tempArr);
            updateColumn(l,tempSudoku[k][l],res,tempArr);
        }
    }
    return res;
}

const generateSolvableUniqueSudoku=(tempSudoku,canRemove,totalRemoved)=>{
    if(totalRemoved===totalToRemove){
        uniqueSolvableSudoku=makeCopyof2DArr(tempSudoku);
        return true;
    }
    permute(canRemove);
    let k,len;
    len=canRemove.length;
    for(k=0;k<len;k++){
        const boxToBeRemoved=canRemove[k];
        const i=Math.floor(boxToBeRemoved/n);
        const j=boxToBeRemoved%n;
        const orVal=tempSudoku[i][j];
        tempSudoku[i][j]=0;
        
        const cpyTempSudoku=makeCopyof2DArr(tempSudoku);
        const possDigitsForBoxij=findPossDigitsForBoxij(cpyTempSudoku);
        multiSolutionExits=false;
        solnExits=false;
        multiSolnExits(cpyTempSudoku,possDigitsForBoxij,totalRemoved+1);
        if(!multiSolutionExits){
            swapElsOfArr(canRemove,k,len-1);
            canRemove.pop();
            return generateSolvableUniqueSudoku(tempSudoku,canRemove,totalRemoved+1);
        }
        tempSudoku[i][j]=orVal;
    }
    return false;
}
const generateRandomSudokuWithUniqueSolution=()=>{
    const tempSudoku=makeCopyof2DArr(completeSudoku);
    const canRemove=initializeCanRemove();

    console.log(generateSolvableUniqueSudoku(tempSudoku,canRemove,0));

    console2DArray(uniqueSolvableSudoku);
}

const generateRandomCompleteSudoku=()=>{
    const possDigsForBoxij=initialPossDigsForBoxij();
    const initialSudoku=initialPartiallyFilledSudoku(possDigsForBoxij);

    console.log(generateSudoku(initialSudoku,possDigsForBoxij,54));

    console2DArray(completeSudoku);
}

const generateUniqueSolvableSudokuWith50Blanks=()=>{
    generateRandomCompleteSudoku();
    generateRandomSudokuWithUniqueSolution();
}
generateUniqueSolvableSudokuWith50Blanks();