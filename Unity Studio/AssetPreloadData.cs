﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Unity_Studio
{
    public class AssetPreloadData : ListViewItem
    {
        public long m_PathID;
        public int Offset;
        public int Size;
        public int Type1;
        public ushort Type2;

        public string TypeString;
        public int exportSize;
        public string InfoText;
        public string extension;

        public AssetsFile sourceFile;
        public string uniqueID;
    }
}
