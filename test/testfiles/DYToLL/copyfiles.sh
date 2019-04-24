##############################################################
# copyfiles.sh
# 
# Copy files using xrdcp to local directory. 
##############################################################

declare -a arr=(
"/store/mc/PhaseIIMTDTDRAutumn18DR/DYToLL_M-50_14TeV_TuneCP5_pythia8/FEVT/PU200_103X_upgrade2023_realistic_v2-v2/910000/F37B1F94-AD89-DF40-8D40-3EE87E8339A1.root"
"/store/mc/PhaseIIMTDTDRAutumn18DR/DYToLL_M-50_14TeV_TuneCP5_pythia8/FEVT/PU200_103X_upgrade2023_realistic_v2-v2/910000/E4590EF0-336D-CB48-B5D8-2349E8573FCF.root"
"/store/mc/PhaseIIMTDTDRAutumn18DR/DYToLL_M-50_14TeV_TuneCP5_pythia8/FEVT/PU200_103X_upgrade2023_realistic_v2-v2/910000/9D485410-BBF7-D841-9048-E273729D9E24.root"
"/store/mc/PhaseIIMTDTDRAutumn18DR/DYToLL_M-50_14TeV_TuneCP5_pythia8/FEVT/PU200_103X_upgrade2023_realistic_v2-v2/910000/97A38B8C-B7B7-C041-AC10-B2794F5B81CB.root"
"/store/mc/PhaseIIMTDTDRAutumn18DR/DYToLL_M-50_14TeV_TuneCP5_pythia8/FEVT/PU200_103X_upgrade2023_realistic_v2-v2/910000/3C7584B3-CD5A-854E-A181-0EB4F5347343.root"
"/store/mc/PhaseIIMTDTDRAutumn18DR/DYToLL_M-50_14TeV_TuneCP5_pythia8/FEVT/PU200_103X_upgrade2023_realistic_v2-v2/910000/196CD577-8C94-794D-AC1A-211D5DC08FA7.root"
"/store/mc/PhaseIIMTDTDRAutumn18DR/DYToLL_M-50_14TeV_TuneCP5_pythia8/FEVT/PU200_103X_upgrade2023_realistic_v2-v2/90003/FF9FF5B4-CEDB-5D4F-8979-D625E27AE0BD.root"
)


for fullfile in "${arr[@]}"

do
    echo "Attempting to copy $fullfile"
    xrdcp root://cms-xrd-global.cern.ch/"$fullfile" .
done